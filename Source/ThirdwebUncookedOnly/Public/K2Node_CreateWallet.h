// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node.h"
#include "K2Node_BaseAsyncTask.h"

#include "GameFramework/GameMode.h"

#include "K2Node_CreateWallet.generated.h"

namespace CwPins
{
	extern const FName Type;
	extern const FName Source;
	extern const FName Provider;
	extern const FName PartnerId; 
	extern const FName Wallet;
	extern const FName Success;
	extern const FName Failed;
	extern const FName Error;
}

UCLASS()
class THIRDWEBUNCOOKEDONLY_API UK2Node_CreateWallet : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()

public:
	
	// UEdGraphNode interface implementation
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	// End of implementation
	
	// UK2Node interface implementation
	virtual FText GetMenuCategory() const override;
	// End of implementation
	
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

protected:
	UEdGraphPin* GetTypePin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::Type);
		check(Pin == NULL || Pin->Direction == EGPD_Input);
		return Pin;
	}
	UEdGraphPin* GetSourcePin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::Source);
		check(Pin == NULL || Pin->Direction == EGPD_Input);
		return Pin;
	}
	UEdGraphPin* GetProviderPin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::Provider);
		check(Pin == NULL || Pin->Direction == EGPD_Input);
		return Pin;
	}
};
