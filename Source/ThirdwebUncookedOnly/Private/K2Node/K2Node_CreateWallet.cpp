// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_CreateWallet.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "ThirdwebRuntimeSettings.h"
#include "TWUOCommon.h"
#include "TWUOUtils.h"

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreateSmartWallet.h"

#include "Styling/SlateIconFinder.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

#define LOCTEXT_NAMESPACE "ThirdwebUncookedOnly"

namespace CwPins
{
	const FName Type = FName(TEXT("Type"));
	const FName Source = FName(TEXT("Source"));
	const FName Provider = FName(TEXT("Provider"));
	const FName AuthInput = FName(TEXT("Auth Input"));
	const FName PartnerId = FName(TEXT("Partner ID"));
	const FName Wallet = FName(TEXT("Wallet"));
	const FName Success = FName(TEXT("Success"));
	const FName Failed = FName(TEXT("Failed"));
	const FName Error = FName(TEXT("Error"));

	// Smart Wallet Pins
	const FName InAppWallet = FName(TEXT("InApp Wallet"));
	const FName SmartWallet = FName(TEXT("Smart Wallet"));
	const FName ChainID = FName(TEXT("Chain ID"));
	const FName Gasless = FName(TEXT("Gasless"));
	const FName Factory = FName(TEXT("Factory"));
	const FName AccountOverride = FName(TEXT("Account Override"));
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
	}
	UpdatePins();
}

FText UK2Node_CreateWallet::GetMenuCategory() const
{
	return LOCTEXT("K2Node_CreateWallet_Category", "Thirdweb|Wallets|InApp");
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
	SetNotConnectable(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebOAuthProvider>(), CwPins::Provider));
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, CwPins::AuthInput);

	// In App Wallet Output Pins
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), CwPins::Wallet);

	// Smart Wallet Input Pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), CwPins::InAppWallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int64, CwPins::ChainID);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, CwPins::Gasless);
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
				ProxyClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateSmartWallet, CreateSmartWallet);
			}
		case EThirdwebInAppWalletSource::Email:
			{
				ProxyClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateSmartWallet, CreateSmartWallet);
			}
		case EThirdwebInAppWalletSource::Phone:
			{
				ProxyClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateSmartWallet, CreateSmartWallet);
			}
		case EThirdwebInAppWalletSource::Jwt:
			{
				ProxyClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateSmartWallet, CreateSmartWallet);
			}
		case EThirdwebInAppWalletSource::AuthEndpoint:
			{
				ProxyClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateSmartWallet, CreateSmartWallet);
			}
		case EThirdwebInAppWalletSource::Guest:
			{
				ProxyClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
				ProxyFactoryClass = UAsyncTaskThirdwebCreateSmartWallet::StaticClass();
				ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebCreateSmartWallet, CreateSmartWallet);
			}
		}
	}

	Super::ExpandNode(CompilerContext, SourceGraph);;
}

FString UK2Node_CreateWallet::ResolvePinValue(UEdGraphPin* Pin)
{
	if (Pin)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResolvePinValue::Pin=Exists|Connections=%d"), Pin->LinkedTo.Num())
		if (UEdGraphPin* ConnectedPin = Pin->LinkedTo.Num() > 0 ? Pin->LinkedTo[0] : nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("ResolvePinValue::Connected Pin Exists"))
			return ConnectedPin->DefaultValue;
		}
		return Pin->DefaultValue;
	}
	return FString();
}

void UK2Node_CreateWallet::UpdatePins()
{
	if (UEdGraphPin* Pin = GetTypePin())
	{
		bool bSmart = ResolvePinValue(Pin) == TEXT("Smart");
		SetHasAdvanced(UThirdwebRuntimeSettings::IsEcosystem() || bSmart);
		FString Source = ResolvePinValue(GetSourcePin());
		UE_LOG(LogTemp, Warning, TEXT("DefaultValue of Type::%s"), *Source)
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
