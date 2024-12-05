// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node.h"
#include "K2Node_ThirdwebBaseAsyncTask.h"
#include "K2Node_ThirdwebSignMessage.generated.h"

namespace TwPins
{
	extern const FName Message;
	extern const FName SignedMessage;
}

UCLASS()
class THIRDWEBUNCOOKEDONLY_API UK2Node_ThirdwebSignMessage : public UK2Node_ThirdwebBaseAsyncTask
{
	GENERATED_BODY()

public:
	UK2Node_ThirdwebSignMessage();
	
	// UEdGraphNode interface implementation
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	// End of implementation

	// UK2Node interface implementation
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual FText GetMenuCategory() const override;
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	// End of implementation

protected:
	virtual bool UpdatePins() override;
};
