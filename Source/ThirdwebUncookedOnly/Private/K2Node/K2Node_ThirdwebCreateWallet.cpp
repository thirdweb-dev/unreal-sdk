﻿// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_ThirdwebCreateWallet.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "ThirdwebRuntimeSettings.h"
#include "TWUOCommon.h"
#include "TWUOUtils.h"

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreateAuthEndpointWallet.h"
#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreateEmailWallet.h"
#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreateGuestWallet.h"
#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreateJwtWallet.h"
#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreateOAuthWallet.h"
#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreatePhoneWallet.h"
#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreateSmartWallet.h"
#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebSignInWithOTP.h"

#include "K2Node/ThridwebK2NodeUtils.h"

#include "Styling/SlateIconFinder.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

#define LOCTEXT_NAMESPACE "ThirdwebUncookedOnly"

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

UK2Node_CreateWallet::UK2Node_CreateWallet()
{
	ProxyClass = UAsyncTaskThirdwebCreateOAuthWallet::StaticClass();
	ProxyFactoryClass = UAsyncTaskThirdwebCreateOAuthWallet::StaticClass();
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateOAuthWallet, CreateOAuthWallet);
}

FText UK2Node_CreateWallet::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (CachedNodeTitle.IsOutOfDate(this))
	{
		CachedNodeTitle.MarkDirty();

		if (UEdGraphPin* TypePin = GetTypePin())
		{
			if (ResolvePinValue(TypePin) == TEXT("Smart"))
			{
				CachedNodeTitle.SetCachedText(LOCTEXT("K2Node_CreateWallet_SmartNodeTitle", "Create Smart Wallet"), this);
			}
			else
			{
				if (FString Source = ResolvePinValue(GetSourcePin()); Source == TEXT("OAuth") || Source == TEXT("Email") || Source == TEXT("Phone"))
				{
					if (Source == TEXT("OAuth"))
					{
						CachedNodeTitle.SetCachedText(
							FText::Format(LOCTEXT("K2Node_CreateWallet_OAuthNodeTitle", "Create In App {0} OAuth Wallet"), FText::FromString(ResolvePinValue(GetProviderPin()))),
							this
						);
					}
					else
					{
						CachedNodeTitle.SetCachedText(
							FText::Format(LOCTEXT("K2Node_CreateWallet_OTPNodeTitle", "Create In App {0} OTP Wallet"), FText::FromString(Source)),
							this
						);
					}
				}
				else
				{
					CachedNodeTitle.SetCachedText(
						FText::Format(LOCTEXT("K2Node_CreateWallet_OTPNodeTitle", "Create In App {0} Wallet"), FText::FromString(Source)),
						this
					);
				}
			}
		}
	}
	return CachedNodeTitle;
	// return LOCTEXT("K2Node_CreateWallet_NodeTitle", "Create In App Wallet");
}

FText UK2Node_CreateWallet::GetTooltipText() const
{
	return LOCTEXT("K2Node_CreateWallet_TooltipText", "Create a Thirdweb Wallet");
}

FLinearColor UK2Node_CreateWallet::GetNodeTitleColor() const
{
	return FLinearColor(FColor::FromHex(TEXT("#7207b9")));
}

FSlateIcon UK2Node_CreateWallet::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = FLinearColor(FColor::FromHex(TEXT("#f213a4")));
	static const FSlateIcon Icon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");;
	return Icon;
}

void UK2Node_CreateWallet::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::PinDefaultValueChanged(Pin);
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

FText UK2Node_CreateWallet::GetMenuCategory() const
{
	return LOCTEXT("K2Node_CreateWallet_Category", "Thirdweb|Wallets|InApp");
}

void UK2Node_CreateWallet::PostReconstructNode()
{
	Super::PostReconstructNode();
	UpdatePins();
}

void UK2Node_CreateWallet::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);
	UpdatePins();
}

void UK2Node_CreateWallet::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions(ActionRegistrar);
	UClass* Action = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(Action))
	{
		UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(GetClass());
		ActionRegistrar.AddBlueprintAction(Action, Spawner);
	}
}

void UK2Node_CreateWallet::AllocateDefaultPins()
{
	// Execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, CwPins::Success);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, CwPins::Failed);

	// Base Selector Pin
	SetNotConnectable(
		SetFriendlyName(
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebWalletType>(), CwPins::Type),
			LOCTEXT("K2Node_CreateWallet_ThirdwebWalletType", "Kind")
		)
	);

	// In App Wallet Input Pins
	SetNotConnectable(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebInAppWalletSource>(), CwPins::Source));
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebOAuthProvider>(), CwPins::Provider);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, CwPins::AuthInput);

	// In App Wallet Output Pins
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), CwPins::Wallet);

	// Smart Wallet Input Pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), CwPins::InAppWallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int64, CwPins::ChainID);
	SetDefaultValue(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, CwPins::Gasless), true);
	SetAdvancedView(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, CwPins::Factory));
	SetAdvancedView(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, CwPins::AccountOverride));

	// Smart Wallet Output Pins
	SetFriendlyName(
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, FSmartWalletHandle::StaticStruct(), CwPins::SmartWallet),
		LOCTEXT("K2Node_CreateWallet_ThirdwebSmartWalletOutput", "Wallet")
	);

	// Ecosystem Input Pin
	SetAdvancedView(SetPinVisibility(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, CwPins::PartnerId), true));

	// Error Output Pin
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_String, CwPins::Error);

	UpdatePins();
}

void UK2Node_CreateWallet::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
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
	K2NodeUtils::Pins::RemoveHidden(this);
	
	Super::ExpandNode(CompilerContext, SourceGraph);
}

FString UK2Node_CreateWallet::ResolvePinValue(UEdGraphPin* Pin)
{
	return Pin ? Pin->LinkedTo.Num() > 0 ? Pin->LinkedTo[0]->DefaultValue : Pin->DefaultValue : FString();
}

void UK2Node_CreateWallet::UpdatePins()
{
	if (UEdGraphPin* Pin = GetTypePin())
	{
		bool bSmart = ResolvePinValue(Pin) == TEXT("Smart");
		SetHasAdvanced(UThirdwebRuntimeSettings::IsEcosystem() || bSmart);
		FString Source = ResolvePinValue(GetSourcePin());
		SetPinVisibility(GetSourcePin(), !bSmart);
		SetPinVisibility(GetProviderPin(), !bSmart && Source == TEXT("OAuth"));
		SetFriendlyName(
			SetPinVisibility(GetAuthInputPin(), !bSmart && (Source == TEXT("Phone") || Source == TEXT("Email"))),
			Source == TEXT("Phone") ? LOCTEXT("K2Node_CreateWallet_ThirdwebPhone", "Phone Number") : LOCTEXT("K2Node_CreateWallet_ThirdwebEmail", "Email Address")
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

#undef LOCTEXT_NAMESPACE