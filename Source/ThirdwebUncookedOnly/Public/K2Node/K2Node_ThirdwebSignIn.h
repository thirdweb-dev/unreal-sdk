// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node.h"
#include "K2Node_ThirdwebBaseAsyncTask.h"
#include "K2Node_ThirdwebSignIn.generated.h"

namespace SiPins
{
	extern const FName Input;
	extern const FName Wallet;
	extern const FName Success;
	extern const FName Failed;
	extern const FName Error;
	
}

UCLASS()
class THIRDWEBUNCOOKEDONLY_API UK2Node_ThirdwebSignIn : public UK2Node_ThirdwebBaseAsyncTask
{
	GENERATED_BODY()

public:
	UK2Node_ThirdwebSignIn();
	
	// UEdGraphNode interface implementation
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	// End of implementation

	// UK2Node interface implementation
	virtual FText GetMenuCategory() const override;
	// End of implementation
	
	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

protected:
	virtual void UpdatePins() override;

	// Get Pins
	UEdGraphPin* GetWalletPin() const;
	UEdGraphPin* GetInputPin() const;
	UEdGraphPin* GetErrorPin() const;
};
