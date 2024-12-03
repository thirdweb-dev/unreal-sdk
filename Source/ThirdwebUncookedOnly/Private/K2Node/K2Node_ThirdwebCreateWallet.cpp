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
	ProxyClass = UAsyncTaskThirdwebCreateOAuthWallet::StaticClass();
	ProxyFactoryClass = UAsyncTaskThirdwebCreateOAuthWallet::StaticClass();
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateOAuthWallet, CreateOAuthWallet);
}

FText UK2Node_ThirdwebCreateWallet::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::MenuTitle)
	{
		return LOCTEXT("K2Node_ThirdwebCreateWallet_SmartNodeTitle", "Create Wallet");
	}
	if (CachedNodeTitle.IsOutOfDate(this))
	{
		CachedNodeTitle.MarkDirty();

		if (UEdGraphPin* TypePin = GetTypePin())
		{
			if (ResolvePinValue(TypePin) == TEXT("Smart"))
			{
				CachedNodeTitle.SetCachedText(LOCTEXT("K2Node_ThirdwebCreateWallet_SmartNodeTitle", "Create Smart Wallet"), this);
			}
			else
			{
				FText OAuthType = FText::FromString(UThirdwebRuntimeSettings::IsEcosystem() ? TEXT("Ecosystem") : TEXT("InApp"));
				
				if (const FString Source = ResolvePinValue(GetSourcePin()); Source == TEXT("OAuth") || Source == TEXT("Email") || Source == TEXT("Phone"))
				{
					if (Source == TEXT("OAuth"))
					{
						CachedNodeTitle.SetCachedText(
							FText::Format(LOCTEXT("K2Node_ThirdwebCreateWallet_OAuthNodeTitle", "Create {1} OAuth {0} Wallet"), OAuthType, FText::FromString(ResolvePinValue(GetProviderPin()))),
							this
						);
					}
					else
					{
						CachedNodeTitle.SetCachedText(
							FText::Format(LOCTEXT("K2Node_ThirdwebCreateWallet_OTPNodeTitle", "Create {1} OTP {0} Wallet"), OAuthType, FText::FromString(Source)),
							this
						);
					}
				}
				else
				{
					CachedNodeTitle.SetCachedText(
						FText::Format(LOCTEXT("K2Node_ThirdwebCreateWallet_OTPNodeTitle", "Create {1} {0} Wallet"),OAuthType,  FText::FromString(Source)),
						this
					);
				}
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
	if (Pin == GetTypePin() || Pin == GetSourcePin() || Pin == GetProviderPin())
	{
		CachedNodeTitle.MarkDirty();
		if (Pin == GetTypePin())
		{
			for (UEdGraphPin* P : Pins)
			{
				if (P->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec && P->PinName != TwPins::Error)
				{
					P->BreakAllPinLinks();
				}
			}
		}
	}
	UpdatePins();
}

void UK2Node_ThirdwebCreateWallet::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	// Base Selector Pin
	SetPinConnectable(
		SetPinFriendlyName(
			SetPinDefaultValue(
				CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebWalletType>(), TwPins::Type),
				TEXT("InApp")
			),
			LOCTEXT("K2Node_ThirdwebCreateWallet_ThirdwebWalletType", "Kind")
		)
	);

	// In App Wallet Input Pins
	SetPinConnectable(
		SetPinDefaultValue(
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebInAppWalletSource>(), TwPins::Source),
			TEXT("OAuth")
		)
	);

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
	bool bSmart = ResolvePinValue(GetTypePin()) == TEXT("Smart");
	if (bSmart)
	{
		ProxyClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
		ProxyFactoryClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
		ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateSmartWallet, CreateSmartWallet);
	}
	else
	{
		switch (ThirdwebUtils::ToInAppWalletSource(ResolvePinValue(GetSourcePin())))
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
		}
	}
	RemovePin(GetTypePin());
	RemovePin(GetSourcePin());
	RemoveHiddenPins(this);
	Super::ExpandNode(CompilerContext, SourceGraph);
}

void UK2Node_ThirdwebCreateWallet::UpdatePins()
{
	if (UEdGraphPin* Pin = GetTypePin())
	{
		bool bSmart = ResolvePinValue(Pin) == TEXT("Smart");
		SetNodeHasAdvanced(bSmart);
		FString Source = ResolvePinValue(GetSourcePin());
		SetPinVisibility(GetSourcePin(), !bSmart);
		SetPinVisibility(GetProviderPin(), !bSmart && Source == TEXT("OAuth"));
		SetPinFriendlyName(
			SetPinVisibility(GetInputPin(), !bSmart && (Source == TEXT("Phone") || Source == TEXT("Email"))),
			Source == TEXT("Phone") ? LOCTEXT("K2Node_ThirdwebCreateWallet_ThirdwebPhone", "Phone Number") : LOCTEXT("K2Node_ThirdwebCreateWallet_ThirdwebEmail", "Email Address")
		);
		SetPinVisibility(GetWalletPin(), !bSmart);
		SetPinVisibility(GetInAppWalletPin(), bSmart);
		SetPinVisibility(GetChainIDPin(), bSmart);
		SetPinVisibility(GetGaslessPin(), bSmart);
		SetPinVisibility(GetFactoryPin(), bSmart);
		SetPinVisibility(GetAccountOverridePin(), bSmart);
		SetPinVisibility(GetSmartWalletPin(), bSmart);
		
		if (UEdGraph* Graph = GetGraph())
		{
			Graph->NotifyGraphChanged();
		}
	}
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetTypePin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::Type);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetSourcePin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::Source);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
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
