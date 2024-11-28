// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node.h"
#include "K2Node_ThirdwebBaseAsyncTask.h"
#include "K2Node_ThirdwebFetchIpfsData.generated.h"

namespace TwPins
{
	extern const FName Uri;
	extern const FName Data;
}

UCLASS()
class THIRDWEBUNCOOKEDONLY_API UK2Node_ThirdwebFetchIpfsData : public UK2Node_ThirdwebBaseAsyncTask
{
	GENERATED_BODY()

public:
	UK2Node_ThirdwebFetchIpfsData();
	
	// UEdGraphNode interface implementation
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	// End of implementation

	virtual void AllocateDefaultPins() override;
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

protected:
	virtual void UpdatePins() override;

	// Base Pins
	UEdGraphPin* GetUriPin() const;
	UEdGraphPin* GetDataPin() const;
};
