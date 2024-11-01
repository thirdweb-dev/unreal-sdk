// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node.h"
#include "K2Node_ThirdwebBaseAsyncTask.h"
#include "K2Node_ThirdwebCreateWallet.generated.h"

namespace TwPins
{
	extern const FName Type;
	extern const FName Source;
	// Smart Wallet Pins
	extern const FName ChainID;
	extern const FName Gasless;
	extern const FName Factory;
	extern const FName AccountOverride;
}

UCLASS()
class THIRDWEBUNCOOKEDONLY_API UK2Node_ThirdwebCreateWallet : public UK2Node_ThirdwebBaseAsyncTask
{
	GENERATED_BODY()

public:
	UK2Node_ThirdwebCreateWallet();
	
	// UEdGraphNode interface implementation
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	// End of implementation

	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

protected:
	virtual void UpdatePins() override;

	// Base Pins
	UEdGraphPin* GetTypePin() const;
	UEdGraphPin* GetSourcePin() const;
	virtual UEdGraphPin* GetWalletPin() const override;
	
	// Smart Wallet Pins
	UEdGraphPin* GetChainIDPin() const;
	UEdGraphPin* GetGaslessPin() const;
	UEdGraphPin* GetFactoryPin() const;
	UEdGraphPin* GetAccountOverridePin() const;
	virtual UEdGraphPin* GetSmartWalletPin() const override;
};
