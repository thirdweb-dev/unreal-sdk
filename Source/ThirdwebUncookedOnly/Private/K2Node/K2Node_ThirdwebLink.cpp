// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_ThirdwebLink.h"

#include "AsyncTasks/Wallets/InApp/Link/AsyncTaskThirdwebLink.h"

namespace LPins
{
	const FName Input = FName(TEXT("Input"));
	const FName Wallet = FName(TEXT("Wallet"));
	const FName NewWallet = FName(TEXT("NewWallet"));
	const FName Success = FName(TEXT("Success"));
	const FName Failed = FName(TEXT("Failed"));
	const FName Error = FName(TEXT("Error"));
}

#define LOCTEXT_NAMESPACE "ThirdwebUncookedOnly"

UK2Node_ThirdwebLink::UK2Node_ThirdwebLink()
{
	ProxyClass = UAsyncTaskThirdwebLink::StaticClass();
	ProxyFactoryClass = UAsyncTaskThirdwebLink::StaticClass();
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebLink, Link);
}

FText UK2Node_ThirdwebLink::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::MenuTitle)
	{
		return LOCTEXT("K2Node_ThirdwebLink_SmartNodeTitle", "Link Wallet");
	}
	return LOCTEXT("K2Node_ThirdwebLink_SmartNodeTitle", "Link Wallet");
}

FText UK2Node_ThirdwebLink::GetTooltipText() const
{
	return LOCTEXT("K2Node_ThirdwebLink_TooltipText", "Link a Thirdweb Wallet");
}

FText UK2Node_ThirdwebLink::GetMenuCategory() const
{
	return LOCTEXT("K2Node_ThirdwebLink_Category", "Thirdweb|Wallets|InApp");
}

void UK2Node_ThirdwebLink::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::Super::PinDefaultValueChanged(Pin);
	UpdatePins();
}

void UK2Node_ThirdwebLink::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	
	// Input Pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), LPins::Wallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), LPins::NewWallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, LPins::Input);
	
	PostAllocateDefaultPins();
}

#undef LOCTEXT_NAMESPACE
