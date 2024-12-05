// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_ThirdwebCreateWallet.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "ThirdwebRuntimeSettings.h"
#include "TWUOCommon.h"
#include "TWUOUtils.h"
#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateAuthEndpointWallet.h"
#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateEmailWallet.h"
#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateGuestWallet.h"
#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateJwtWallet.h"
#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateOAuthWallet.h"
#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreatePhoneWallet.h"
#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateSiweWallet.h"
#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateSmartWallet.h"
#include "Styling/SlateIconFinder.h"
#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

namespace TwPins
{
	const FName Type = FName(TEXT("Type"));
	const FName Source = FName(TEXT("Source"));
	const FName ChainID = FName(TEXT("ChainID"));
	const FName Gasless = FName(TEXT("bGasless"));
	const FName Factory = FName(TEXT("Factory"));
	const FName AccountOverride = FName(TEXT("AccountOverride"));
}

#define LOCTEXT_NAMESPACE "ThirdwebUncookedOnly"

UK2Node_ThirdwebCreateWallet::UK2Node_ThirdwebCreateWallet()
{
	Type = EThirdwebWalletType::InApp;
	Source = EThirdwebInAppWalletSource::OAuth;
	ProxyClass = UAsyncTaskThirdwebCreateOAuthWallet::StaticClass();
	ProxyFactoryClass = UAsyncTaskThirdwebCreateOAuthWallet::StaticClass();
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateOAuthWallet, CreateOAuthWallet);
}

void UK2Node_ThirdwebCreateWallet::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	// ReSharper disable once CppTooWideScopeInitStatement
	TArray RelevantPins = {GET_MEMBER_NAME_CHECKED(UK2Node_ThirdwebCreateWallet, Source), GET_MEMBER_NAME_CHECKED(UK2Node_ThirdwebCreateWallet, Type)};
	if (RelevantPins.Contains(PropertyChangedEvent.GetMemberPropertyName()))
	{
		if (UpdatePins())
		{
			ReconstructNode();
		}
	}
}

FText UK2Node_ThirdwebCreateWallet::GetNodeTitle(const ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::MenuTitle)
	{
		return LOCTEXT("K2Node_ThirdwebCreateWallet_MenuNodeTitle", "Create Wallet");
	}
	if (CachedNodeTitle.IsOutOfDate(this))
	{
		switch (Type)
		{
		case EThirdwebWalletType::Smart:
			{
				CachedNodeTitle.SetCachedText(LOCTEXT("K2Node_ThirdwebCreateWallet_SmartNodeTitle", "Create Smart Wallet"), this);
				break;
			}
		case EThirdwebWalletType::InApp:
			{
				FText WalletCategory = FText::FromString(UThirdwebRuntimeSettings::IsEcosystem() ? TEXT("Ecosystem") : TEXT("InApp"));
				if (Source == EThirdwebInAppWalletSource::OAuth)
				{
					CachedNodeTitle.SetCachedText(
						FText::Format(LOCTEXT("K2Node_ThirdwebCreateWallet_OAuthNodeTitle", "Create {1} OAuth {0} Wallet"), WalletCategory, FText::FromString(ResolvePinValue(GetProviderPin()))),
						this
					);
				}
				else if (Source == EThirdwebInAppWalletSource::Email || Source == EThirdwebInAppWalletSource::Phone)
				{
					CachedNodeTitle.SetCachedText(
						FText::Format(LOCTEXT("K2Node_ThirdwebCreateWallet_OTPNodeTitle", "Create {1} OTP {0} Wallet"), WalletCategory, ThirdwebUtils::ToText(Source)),
						this
					);
				}
				else
				{
					CachedNodeTitle.SetCachedText(
						FText::Format(LOCTEXT("K2Node_ThirdwebCreateWallet_GeneralNodeTitle", "Create {1} {0} Wallet"), WalletCategory, ThirdwebUtils::ToText(Source)),
						this
					);
				}
				break;
			}
		}
	}
	return CachedNodeTitle;
}

FText UK2Node_ThirdwebCreateWallet::GetTooltipText() const
{
	return LOCTEXT("K2Node_ThirdwebCreateWallet_TooltipText", "Create a Thirdweb Wallet");
}

void UK2Node_ThirdwebCreateWallet::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::Super::PinDefaultValueChanged(Pin);
	if (Pin == GetProviderPin())
	{
		CachedNodeTitle.MarkDirty();
		for (UEdGraphPin* P : Pins)
		{
			if (P->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec && P->PinName != TwPins::Error)
			{
				P->BreakAllPinLinks();
			}
		}
	}
	UpdatePins();
}

void UK2Node_ThirdwebCreateWallet::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	SetPinDefaultValue(
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebOAuthProvider>(), TwPins::Provider),
		TEXT("Google")
	);

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Input);

	// In App Wallet Output Pins
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), TwPins::Wallet);

	// Smart Wallet Input Pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), TwPins::InAppWallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int64, TwPins::ChainID);
	SetPinDefaultValue(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, TwPins::Gasless), TEXT("true"));
	SetPinAdvancedView(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Factory));
	SetPinAdvancedView(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::AccountOverride));

	// Smart Wallet Output Pins
	SetPinFriendlyName(
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, FSmartWalletHandle::StaticStruct(), TwPins::SmartWallet),
		LOCTEXT("K2Node_ThirdwebCreateWallet_ThirdwebSmartWalletOutput", "Wallet")
	);

	PostAllocateDefaultPins();
}

void UK2Node_ThirdwebCreateWallet::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	if (Type == EThirdwebWalletType::Smart)
	{
		ProxyClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
		ProxyFactoryClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
		ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateSmartWallet, CreateSmartWallet);
	}
	else
	{
		switch (Source)
		{
		case EThirdwebInAppWalletSource::OAuth:
			{
				ProxyClass = UAsyncTaskThirdwebCreateOAuthWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateOAuthWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateOAuthWallet, CreateOAuthWallet);
				break;
			}
		case EThirdwebInAppWalletSource::Email:
			{
				ProxyClass = UAsyncTaskThirdwebCreateEmailWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateEmailWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateEmailWallet, CreateEmailWallet);
				break;
			}
		case EThirdwebInAppWalletSource::Phone:
			{
				ProxyClass = UAsyncTaskThirdwebCreatePhoneWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreatePhoneWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreatePhoneWallet, CreatePhoneWallet);
				break;
			}
		case EThirdwebInAppWalletSource::Jwt:
			{
				ProxyClass = UAsyncTaskThirdwebCreateJwtWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateJwtWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateJwtWallet, CreateJwtWallet);
				break;
			}
		case EThirdwebInAppWalletSource::AuthEndpoint:
			{
				ProxyClass = UAsyncTaskThirdwebCreateAuthEndpointWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateAuthEndpointWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateAuthEndpointWallet, CreateAuthEndpointWallet);
				break;
			}
		case EThirdwebInAppWalletSource::Guest:
			{
				ProxyClass = UAsyncTaskThirdwebCreateGuestWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateGuestWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateGuestWallet, CreateGuestWallet);
				break;
			}
		case EThirdwebInAppWalletSource::Siwe:
			{
				ProxyClass = UAsyncTaskThirdwebCreateSiweWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateSiweWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateSiweWallet, CreateSiweWallet);
			}
		default:
			{
				ProxyClass = UAsyncTaskThirdwebCreateOAuthWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateOAuthWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateOAuthWallet, CreateOAuthWallet);
				break;
			}
		}
	}
	RemoveHiddenPins(this);
	Super::ExpandNode(CompilerContext, SourceGraph);
}

bool UK2Node_ThirdwebCreateWallet::UpdatePins()
{
	SetNodeHasAdvanced(Type == EThirdwebWalletType::Smart);
	SetPinVisibility(GetProviderPin(), Type != EThirdwebWalletType::Smart && Source == EThirdwebInAppWalletSource::OAuth);
	SetPinFriendlyName(
		SetPinVisibility(GetInputPin(), Type != EThirdwebWalletType::Smart && (Source == EThirdwebInAppWalletSource::Phone || Source == EThirdwebInAppWalletSource::Email)),
		Source == EThirdwebInAppWalletSource::Phone ? LOCTEXT("K2Node_ThirdwebCreateWallet_ThirdwebPhone", "Phone Number") : LOCTEXT("K2Node_ThirdwebCreateWallet_ThirdwebEmail", "Email Address")
	);
	SetPinVisibility(GetWalletPin(), Type != EThirdwebWalletType::Smart);
	SetPinVisibility(GetInAppWalletPin(), Type == EThirdwebWalletType::Smart);
	SetPinVisibility(GetChainIDPin(), Type == EThirdwebWalletType::Smart);
	SetPinVisibility(GetGaslessPin(), Type == EThirdwebWalletType::Smart);
	SetPinVisibility(GetFactoryPin(), Type == EThirdwebWalletType::Smart);
	SetPinVisibility(GetAccountOverridePin(), Type == EThirdwebWalletType::Smart);
	SetPinVisibility(GetSmartWalletPin(), Type == EThirdwebWalletType::Smart);


	if (UEdGraph* Graph = GetGraph())
	{
		Graph->NotifyGraphChanged();
	}
	return true;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetWalletPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::Wallet);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}

// Smart Wallet Pins

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetChainIDPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::ChainID);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetGaslessPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::Gasless);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetFactoryPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::Factory);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetAccountOverridePin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::AccountOverride);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetSmartWalletPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::SmartWallet);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}

#undef LOCTEXT_NAMESPACE
