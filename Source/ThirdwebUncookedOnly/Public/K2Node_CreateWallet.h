// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node.h"

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
class THIRDWEBUNCOOKEDONLY_API UK2Node_CreateWallet : public UK2Node
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
	
};
