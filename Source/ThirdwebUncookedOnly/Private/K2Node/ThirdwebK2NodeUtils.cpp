// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "BlueprintActionDatabase.h"
#include "K2Node.h"

#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#include "K2Node/ThridwebK2NodeUtils.h"

namespace K2NodeUtils
{
	namespace Pins
	{
		UEdGraphPin* SetVisibility(UEdGraphPin* Pin, const bool bShow)
		{
			if (Pin)
			{
				Pin->bHidden = !bShow;
			}
			return Pin;
		}


		UEdGraphPin* SetFriendlyName(UEdGraphPin* Pin, const FText& FriendlyName)
		{
			if (Pin)
			{
				Pin->PinFriendlyName = FriendlyName;
			}
			return Pin;
		}

		UEdGraphPin* SetConnectable(UEdGraphPin* Pin, const bool bConnectable)
		{
			if (Pin)
			{
				Pin->bNotConnectable = !bConnectable;
			}
			return Pin;
		}

		UEdGraphPin* SetAdvancedView(UEdGraphPin* Pin, const bool bAdvanced)
		{
			if (Pin)
			{
				Pin->bAdvancedView = bAdvanced;
			}
			return Pin;
		}

		void RemoveHidden(UK2Node* K2Node)
		{
			TArray<UEdGraphPin*> Pins = K2Node->Pins;
			for (int i = 0; i < Pins.Num(); i++)
			{
				if (UEdGraphPin* Pin = Pins[i]; Pin && Pin->bHidden)
				{
					UE_LOG(LogTemp, Log, TEXT("K2NodeUtils::DestroyHidden::Destroying unused pin %s"), *Pin->PinName.ToString())
					K2Node->RemovePin(Pin);
				}
			}
		}
	}

	void SetHasAdvanced(UK2Node* K2Node, const bool bHasAdvanced)
	{
		if (K2Node)
		{
			if (bHasAdvanced)
			{
				if (K2Node->AdvancedPinDisplay == ENodeAdvancedPins::NoPins)
				{
					K2Node->AdvancedPinDisplay = ENodeAdvancedPins::Hidden;
				}
			}
			else
			{
				K2Node->AdvancedPinDisplay = ENodeAdvancedPins::NoPins;
			}
		}
	}
}
