// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_ThirdwebLink.h"

#include "AsyncTasks/Wallets/InApp/Link/AsyncTaskThirdwebLink.h"

namespace TwPins
{
	const FName NewWallet = FName(TEXT("NewWallet"));
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
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), TwPins::Wallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), TwPins::NewWallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Input);
	
	PostAllocateDefaultPins();
}

#undef LOCTEXT_NAMESPACE
