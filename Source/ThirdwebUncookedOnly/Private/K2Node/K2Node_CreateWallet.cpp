// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_CreateWallet.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "ThirdwebRuntimeSettings.h"
#include "TWUOCommon.h"

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
				FString Source = ResolvePinValue(GetSourcePin());
				if (Source == TEXT("OAuth") || Source == TEXT("Email") || Source == TEXT("Phone"))
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
		// FString Type = "";
		// if (GetValuePin())
		// {
		// 	if (GetValuePin()->PinType.PinCategory == UEdGraphSchema_K2::PC_Int)
		// 	{
		// 		FString DisplayType = FCustomNamePin::Deserialize(DisplayTypePin->DefaultValue);
		// 		if (DisplayType == "Boost Number") Type = " Boost Number";
		// 		else if (DisplayType == "Boost Percentage") Type = " Boost Percentage";
		// 		else if (DisplayType == "Number") Type = " Simple Number";
		// 		else Type = " Number";
		// 	}
		// 	else if (GetValuePin()->PinType.PinCategory == UEdGraphSchema_K2::PC_Real) Type = " Float";
		// 	else if (GetValuePin()->PinType.PinCategory == UEdGraphSchema_K2::PC_String) Type = " String";
		// 	else if (GetValuePin()->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct) Type = " DateTime";
		// }
		// FString Title = FString::Printf(TEXT("Create%s Attribute"), *Type);
		// CachedNodeTitle.SetCachedText(FText::FromString(Title), this);
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
	Super::ExpandNode(CompilerContext, SourceGraph);

	const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();
	bool bIsErrorFree = true;
	EThirdwebWalletType Type = EThirdwebWalletType::Smart;

	UK2Node_CreateWallet* AsyncTask = CompilerContext.SpawnIntermediateNode<UK2Node_CreateWallet>(this, SourceGraph);
	AsyncTask->AllocateDefaultPins();
	GetSourcePin()->SafeSetHidden(Type != EThirdwebWalletType::InApp);
	//UK2Node_CallFunction* const IsValidProxyObjectNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	//IsValidProxyObjectNode->FunctionReference.SetExternalMember(GET_FUNCTION_NAME_CHECKED(UKismetSystemLibrary, IsValid), UKismetSystemLibrary::StaticClass());
	//IsValidProxyObjectNode->AllocateDefaultPins();
	//
	//UEdGraphPin* IsValidInputPin = IsValidProxyObjectNode->FindPinChecked(TEXT("Object"));
	//bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*FindPin(NAME_ObjectToCheck), *IsValidInputPin).CanSafeConnect();
	////End
	//
	////Luckily there is already a node for the Branch. We just have to spawn the node like we did earlier.
	//UK2Node_IfThenElse* IfThenElseProxyNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
	//
	////Let the Branch node add its pins.
	//IfThenElseProxyNode->AllocateDefaultPins();
	//
	////Now we connect the ReturnValue pin from the spawned IsValid node to the Condition pin on the Branch.
	////MovePinLinksToIntermediate is not used here to link pins. When you connect two spawned nodes with each other, use TryCreateConnection.
	//bIsErrorFree &= Schema->TryCreateConnection(IsValidProxyObjectNode->GetReturnValuePin(), IfThenElseProxyNode->GetConditionPin());
	//
	////Now let's move all the pin links from our visual pins on the node to the pins on the spawned Branch node.
	////First, we move the links on the ExecPin to the spawned Branch's ExecPin.
	//bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *IfThenElseProxyNode->GetExecPin()).CanSafeConnect();
	//
	////Second, we move the links on the IsValidExec pin to the spawned Branch's TrueExecPin.
	//bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*FindPin(NAME_IsValidExec), *IfThenElseProxyNode->GetThenPin()).CanSafeConnect();
	//
	////Third, we move the links on the IsNotValidExec pin to the spawned Branch's FalseExecPin.
	//bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*FindPin(NAME_IsNotValidExec), *IfThenElseProxyNode->GetElsePin()).CanSafeConnect();
	//
	////To make sure all actions were successful, we generate an error to the message log if that's not the case.
	//if (!bIsErrorFree)
	//{
	//	//This is how you create compiler errors for Blueprint. I'll show more examples later.
	//	CompilerContext.MessageLog.Error(*LOCTEXT("InternalConnectionError", "IsValidNode: Internal connection error. @@").ToString(), this);
	//}
	//
	////No idea why you have to do this step, yet. Will update when I do.
	BreakAllNodeLinks();
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
