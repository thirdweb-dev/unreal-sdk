// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node.h"
#include "K2Node_ThirdwebBaseAsyncTask.h"
#include "TWUOCommon.h"
#include "K2Node_ThirdwebSignIn.generated.h"

UCLASS()
class THIRDWEBUNCOOKEDONLY_API UK2Node_ThirdwebSignIn : public UK2Node_ThirdwebBaseAsyncTask
{
	GENERATED_BODY()

public:
	UK2Node_ThirdwebSignIn();
	
	// UEdGraphNode interface implementation
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	// End of implementation

	// UK2Node interface implementation
	virtual FText GetMenuCategory() const override;
	virtual bool ShouldShowNodeProperties() const override { return true; }
	// End of implementation
	
	virtual bool UpdatePins() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Provider")
	EThirdwebInAppWalletSource Source;
};
