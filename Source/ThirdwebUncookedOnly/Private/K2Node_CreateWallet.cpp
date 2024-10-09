// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node_CreateWallet.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "TWUOCommon.h"

#include "Styling/SlateIconFinder.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

#define LOCTEXT_NAMESPACE "ThirdwebUncookedOnly"

namespace CwPins
{
	const FName Type = FName(TEXT("Type"));
	const FName Source = FName(TEXT("Source"));
	const FName Provider = FName(TEXT("Provider"));
	const FName PartnerId = FName(TEXT("Partner ID"));
	const FName Wallet = FName(TEXT("Wallet"));
	const FName Success = FName(TEXT("Success"));
	const FName Failed = FName(TEXT("Failed"));
	const FName Error = FName(TEXT("Error"));
}

FText UK2Node_CreateWallet::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("K2Node_CreateWallet_NodeTitle", "Create In App Wallet");
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
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebWalletType>(), CwPins::Type);
	UEdGraphPin* SourcePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebInAppWalletSource>(), CwPins::Source);

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EThirdwebOAuthProvider>(), CwPins::Provider);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, CwPins::PartnerId);
	
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, CwPins::Success);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), CwPins::Wallet);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, CwPins::Failed);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_String, CwPins::Error);
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

#undef LOCTEXT_NAMESPACE
