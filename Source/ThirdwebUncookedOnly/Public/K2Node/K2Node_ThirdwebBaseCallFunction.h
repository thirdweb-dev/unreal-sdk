// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

#pragma once

#include "K2Node_CallFunction.h"
#include "K2Node_ThirdwebBaseCallFunction.generated.h"


UCLASS()
class THIRDWEBUNCOOKEDONLY_API UK2Node_ThirdwebBaseCallFunction : public UK2Node_CallFunction
{
	GENERATED_BODY()

public:
	// UEdGraphNode interface implementation
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	// End of implementation
	
	// UK2Node interface implementation
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	// End of implementation
};
