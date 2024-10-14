// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_ThirdwebSignMessage.h"

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebInAppSignMessage.h"
#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebSmartSignMessage.h"

#include "Wallets/ThirdwebSmartWalletHandle.h"

namespace TwPins
{
	const FName Message = FName(TEXT("Message"));
	const FName SignedMessage = FName(TEXT("SignedMessage"));
}

#define LOCTEXT_NAMESPACE "ThirdwebUncookedOnly"

UK2Node_ThirdwebSignMessage::UK2Node_ThirdwebSignMessage()
{
	ProxyClass = UAsyncTaskThirdwebInAppSignMessage::StaticClass();
	ProxyFactoryClass = UAsyncTaskThirdwebInAppSignMessage::StaticClass();
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebInAppSignMessage, SignMessage);
}

FText UK2Node_ThirdwebSignMessage::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("K2Node_ThirdwebSignMessage_SmartNodeTitle", "Sign Message");
}

FText UK2Node_ThirdwebSignMessage::GetTooltipText() const
{
	return LOCTEXT("K2Node_ThirdwebSignMessage_TooltipText", "Sign a Message with a Thirdweb Wallet");
}

FText UK2Node_ThirdwebSignMessage::GetMenuCategory() const
{
	return LOCTEXT("K2Node_ThirdwebSignMessage_Category", "Thirdweb|Wallets");
}

bool UK2Node_ThirdwebSignMessage::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const
{
	if (MyPin == GetWalletPin() && OtherPin)
	{
		if (OtherPin->PinType.PinSubCategoryObject != TBaseStructure<FInAppWalletHandle>::Get() && OtherPin->PinType.PinSubCategoryObject != TBaseStructure<FSmartWalletHandle>::Get())
		{
			OutReason = LOCTEXT("K2Node_ThirdwebSignMessage_IsConnectionDisallowed", "Only In App & Smart Wallet Handles allowed").ToString();
			return true;
		}
	}
	return Super::IsConnectionDisallowed(MyPin, OtherPin, OutReason);
}

void UK2Node_ThirdwebSignMessage::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::Super::PinDefaultValueChanged(Pin);
	UpdatePins();
}

void UK2Node_ThirdwebSignMessage::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	// Input Pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, TwPins::Wallet);

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Message);

	// Output Pins
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_String, TwPins::SignedMessage);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_String, TwPins::Error);

	UpdatePins();
}

void UK2Node_ThirdwebSignMessage::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	if (UEdGraphPin* Pin = GetWalletPin())
	{
		if (Pin->PinType.PinSubCategoryObject == TBaseStructure<FSmartWalletHandle>::Get())
		{
			ProxyClass = UAsyncTaskThirdwebSmartSignMessage::StaticClass();
			ProxyFactoryClass = UAsyncTaskThirdwebSmartSignMessage::StaticClass();
			ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebSmartSignMessage, SignMessage);
		}
		else
		{
			ProxyClass = UAsyncTaskThirdwebInAppSignMessage::StaticClass();
			ProxyFactoryClass = UAsyncTaskThirdwebInAppSignMessage::StaticClass();
			ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebInAppSignMessage, SignMessage);
		}
	}
	Super::ExpandNode(CompilerContext, SourceGraph);
}

void UK2Node_ThirdwebSignMessage::UpdatePins()
{
	if (UEdGraphPin* Pin = GetWalletPin())
	{
		if (UEdGraphPin* ConnectedPin = GetConnectedPin(Pin))
		{
			if (Pin->PinType != ConnectedPin->PinType)
			{
				Pin->PinType = ConnectedPin->PinType;
				GetGraph()->NotifyGraphChanged();
			}
		}
		else
		{
			if (Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard)
			{
				Pin->PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
				Pin->BreakAllPinLinks();
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
