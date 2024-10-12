// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_ThirdwebSignIn.h"

#include "AsyncTasks/Wallets/InApp/SignIn/AsyncTaskThirdwebSignIn.h"

namespace SiPins
{
	const FName Input = FName(TEXT("Input"));
	const FName Wallet = FName(TEXT("Wallet"));
	const FName Success = FName(TEXT("Success"));
	const FName Failed = FName(TEXT("Failed"));
	const FName Error = FName(TEXT("Error"));
}

#define LOCTEXT_NAMESPACE "ThirdwebUncookedOnly"

UK2Node_ThirdwebSignIn::UK2Node_ThirdwebSignIn()
{
	ProxyClass = UAsyncTaskThirdwebSignIn::StaticClass();
	ProxyFactoryClass = UAsyncTaskThirdwebSignIn::StaticClass();
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebSignIn, SignIn);
}

FText UK2Node_ThirdwebSignIn::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::MenuTitle)
	{
		return LOCTEXT("K2Node_ThirdwebSignIn_SmartNodeTitle", "Sign In");
	}
	return LOCTEXT("K2Node_ThirdwebSignIn_SmartNodeTitle", "Sign In");
}

FText UK2Node_ThirdwebSignIn::GetTooltipText() const
{
	return LOCTEXT("K2Node_ThirdwebSignIn_TooltipText", "Sign In With a Thirdweb Wallet");
}

FText UK2Node_ThirdwebSignIn::GetMenuCategory() const
{
	return LOCTEXT("K2Node_ThirdwebSignIn_Category", "Thirdweb|Wallets|InApp");
}

void UK2Node_ThirdwebSignIn::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::Super::PinDefaultValueChanged(Pin);
	UpdatePins();
}

void UK2Node_ThirdwebSignIn::AllocateDefaultPins()
{
	// Execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, SiPins::Success);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, SiPins::Failed);
	
	// Input Pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), SiPins::Wallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, SiPins::Input);
	
	// Output Pins
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_String, SiPins::Error);
	
	UpdatePins();
}

void UK2Node_ThirdwebSignIn::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	UE_LOG(LogTemp, Log, TEXT("UK2Node_ThirdwebSignIn::ExpandNode::WalletValue::%s"), *ResolvePinValue(GetWalletPin()));
	Super::ExpandNode(CompilerContext, SourceGraph);
}

void UK2Node_ThirdwebSignIn::UpdatePins()
{
	
}

UEdGraphPin* UK2Node_ThirdwebSignIn::GetWalletPin() const
{
	UEdGraphPin* Pin = FindPin(SiPins::Wallet);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebSignIn::GetInputPin() const
{
	UEdGraphPin* Pin = FindPin(SiPins::Input);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebSignIn::GetErrorPin() const
{
	UEdGraphPin* Pin = FindPin(SiPins::Error);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}

#undef LOCTEXT_NAMESPACE
