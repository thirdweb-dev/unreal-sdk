// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node.h"
#include "K2Node_ThirdwebBaseAsyncTask.h"
#include "K2Node_ThirdwebLink.generated.h"

namespace TwPins
{
	extern const FName NewWallet;
}

UCLASS()
class THIRDWEBUNCOOKEDONLY_API UK2Node_ThirdwebLink : public UK2Node_ThirdwebBaseAsyncTask
{
	GENERATED_BODY()

public:
	UK2Node_ThirdwebLink();
	
	// UEdGraphNode interface implementation
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	// End of implementation

	// UK2Node interface implementation
	virtual FText GetMenuCategory() const override;
	// End of implementation
	
	virtual void AllocateDefaultPins() override;
};
