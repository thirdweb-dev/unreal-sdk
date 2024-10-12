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
#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateSmartWallet.h"

#include "Styling/SlateIconFinder.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

namespace CwPins
{
	const FName Type = FName(TEXT("Type"));
	const FName Source = FName(TEXT("Source"));
	const FName Provider = FName(TEXT("Provider"));
	const FName AuthInput = FName(TEXT("Input"));
	const FName PartnerId = FName(TEXT("PartnerId"));
	const FName Wallet = FName(TEXT("Wallet"));
	const FName Success = FName(TEXT("Success"));
	const FName Failed = FName(TEXT("Failed"));
	const FName Error = FName(TEXT("Error"));

	// Smart Wallet Pins
	const FName InAppWallet = FName(TEXT("InAppWallet"));
	const FName SmartWallet = FName(TEXT("SmartWallet"));
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
				if (FString Source = ResolvePinValue(GetSourcePin()); Source == TEXT("OAuth") || Source == TEXT("Email") || Source == TEXT("Phone"))
				{
					if (Source == TEXT("OAuth"))
					{
						CachedNodeTitle.SetCachedText(
							FText::Format(LOCTEXT("K2Node_ThirdwebCreateWallet_OAuthNodeTitle", "Create In App {0} OAuth Wallet"), FText::FromString(ResolvePinValue(GetProviderPin()))),
							this
						);
					}
					else
					{
						CachedNodeTitle.SetCachedText(
							FText::Format(LOCTEXT("K2Node_ThirdwebCreateWallet_OTPNodeTitle", "Create In App {0} OTP Wallet"), FText::FromString(Source)),
							this
						);
					}
				}
				else
				{
					CachedNodeTitle.SetCachedText(
						FText::Format(LOCTEXT("K2Node_ThirdwebCreateWallet_OTPNodeTitle", "Create In App {0} Wallet"), FText::FromString(Source)),
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
				if (P->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec && P->PinName != CwPins::Error)
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
	// Execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, CwPins::Success);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, CwPins::Failed);

	// Base Selector Pin
	SetPinConnectable(
		SetPinFriendlyName(
			SetPinDefaultValue(
				CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebWalletType>(), CwPins::Type),
				TEXT("InApp")
			),
			LOCTEXT("K2Node_ThirdwebCreateWallet_ThirdwebWalletType", "Kind")
		)
	);

	// In App Wallet Input Pins
	SetPinConnectable(
		SetPinDefaultValue(
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebInAppWalletSource>(), CwPins::Source),
			TEXT("OAuth")
		)
	);

	SetPinDefaultValue(
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebOAuthProvider>(), CwPins::Provider),
		TEXT("Google")
	);
	
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, CwPins::AuthInput);

	// In App Wallet Output Pins
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), CwPins::Wallet);

	// Smart Wallet Input Pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), CwPins::InAppWallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int64, CwPins::ChainID);
	SetPinDefaultValue(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, CwPins::Gasless), TEXT("true"));
	SetPinAdvancedView(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, CwPins::Factory));
	SetPinAdvancedView(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, CwPins::AccountOverride));

	// Smart Wallet Output Pins
	SetPinFriendlyName(
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, FSmartWalletHandle::StaticStruct(), CwPins::SmartWallet),
		LOCTEXT("K2Node_ThirdwebCreateWallet_ThirdwebSmartWalletOutput", "Wallet")
	);

	// Ecosystem Input Pin
	SetPinAdvancedView(SetPinVisibility(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, CwPins::PartnerId), true));

	// Error Output Pin
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_String, CwPins::Error);

	UpdatePins();
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
		SetNodeHasAdvanced(UThirdwebRuntimeSettings::IsEcosystem() || bSmart);
		FString Source = ResolvePinValue(GetSourcePin());
		SetPinVisibility(GetSourcePin(), !bSmart);
		SetPinVisibility(GetProviderPin(), !bSmart && Source == TEXT("OAuth"));
		SetPinFriendlyName(
			SetPinVisibility(GetAuthInputPin(), !bSmart && (Source == TEXT("Phone") || Source == TEXT("Email"))),
			Source == TEXT("Phone") ? LOCTEXT("K2Node_ThirdwebCreateWallet_ThirdwebPhone", "Phone Number") : LOCTEXT("K2Node_ThirdwebCreateWallet_ThirdwebEmail", "Email Address")
		);
		SetPinVisibility(GetWalletPin(), !bSmart);
		SetPinVisibility(GetInAppWalletPin(), bSmart);
		SetPinVisibility(GetChainIDPin(), bSmart);
		SetPinVisibility(GetGaslessPin(), bSmart);
		SetPinVisibility(GetFactoryPin(), bSmart);
		SetPinVisibility(GetAccountOverridePin(), bSmart);
		SetPinVisibility(GetSmartWalletPin(), bSmart);

		SetPinVisibility(GetPartnerIDPin(), UThirdwebRuntimeSettings::IsEcosystem());

		if (UEdGraph* Graph = GetGraph())
		{
			Graph->NotifyGraphChanged();
		}
	}
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetTypePin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::Type);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetSourcePin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::Source);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetProviderPin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::Provider);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetAuthInputPin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::AuthInput);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetWalletPin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::Wallet);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetErrorPin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::Error);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}

// Smart Wallet Pins
UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetInAppWalletPin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::InAppWallet);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetChainIDPin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::ChainID);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetGaslessPin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::Gasless);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetFactoryPin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::Factory);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetAccountOverridePin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::AccountOverride);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetSmartWalletPin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::SmartWallet);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}

// Ecosystem Pin
UEdGraphPin* UK2Node_ThirdwebCreateWallet::GetPartnerIDPin() const
{
	UEdGraphPin* Pin = FindPin(CwPins::PartnerId);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

#undef LOCTEXT_NAMESPACE
