﻿// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node_BaseAsyncTask.h"
#include "K2Node_ThirdwebBaseAsyncTask.generated.h"

namespace TwPins
{
	extern const FName Provider;
	extern const FName Input;
	extern const FName Wallet;
	extern const FName InAppWallet;
	extern const FName SmartWallet;
	extern const FName Signature;
	extern const FName Success;
	extern const FName Failed;
	extern const FName Error;
}

UCLASS(Abstract)
class THIRDWEBUNCOOKEDONLY_API UK2Node_ThirdwebBaseAsyncTask : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()

protected:
	FNodeTextCache CachedNodeTitle;

public:
	UK2Node_ThirdwebBaseAsyncTask();
	
	// UEdGraphNode interface implementation
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	// End of implementation


	// UK2Node interface implementation
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual FText GetMenuCategory() const override;
	virtual void PostReconstructNode() override;
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	// End of implementation

	virtual void AllocateDefaultPins() override;
	virtual void PostAllocateDefaultPins();

protected:
	/** Set the value of `bHidden` on the pin */
	static UEdGraphPin* SetPinVisibility(UEdGraphPin* Pin, const bool bShow);
	
	/** Set the value of `PinFriendlyName` on the pin */
	static UEdGraphPin* SetPinFriendlyName(UEdGraphPin* Pin, const FText& FriendlyName);
	
	/** Set the value of `bNotConnectable` to true on the pin */
	static UEdGraphPin* SetPinConnectable(UEdGraphPin* Pin, const bool bConnectable = false);
	
	/** Set the value of `bAdvancedView` on the pin */
	static UEdGraphPin* SetPinAdvancedView(UEdGraphPin* Pin, const bool bAdvanced = true);
	
	/** Destroy all hidden pins */
	static void RemoveHiddenPins(UK2Node* K2Node);

	/** Get the pin connected to the current pin */
	static UEdGraphPin* GetConnectedPin(UEdGraphPin* Pin);
	
	/** Get the resolved value of the pin */
	static FString ResolvePinValue(UEdGraphPin* Pin);
	
	/** Set the value of `AutogeneratedDefaultValue` on the pin */
	static UEdGraphPin* SetPinDefaultValue(UEdGraphPin* Pin, const FString& Value);
	
	/** Update All pins based on logic */
	virtual void UpdatePins() {}
	/** Set the value of `AdvancedPinDisplay` on the node */
	void SetNodeHasAdvanced(const bool bHasAdvanced);


	/** Pin Getters */
	virtual UEdGraphPin* GetProviderPin() const;
	virtual UEdGraphPin* GetInputPin() const;
	virtual UEdGraphPin* GetWalletPin() const;
	virtual UEdGraphPin* GetInAppWalletPin() const;
	virtual UEdGraphPin* GetSmartWalletPin() const;
	virtual UEdGraphPin* GetSuccessPin() const;
	virtual UEdGraphPin* GetFailedPin() const;
	virtual UEdGraphPin* GetErrorPin() const;
};
