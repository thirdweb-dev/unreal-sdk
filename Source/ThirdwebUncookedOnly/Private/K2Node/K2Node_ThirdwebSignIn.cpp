// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_ThirdwebSignIn.h"

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebSignIn.h"


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
	Super::AllocateDefaultPins();
	
	// Input Pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), TwPins::Wallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Input);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Signature);
	PostAllocateDefaultPins();
}

#undef LOCTEXT_NAMESPACE
