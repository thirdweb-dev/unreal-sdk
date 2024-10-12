// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node.h"
#include "K2Node_ThirdwebBaseAsyncTask.h"
#include "K2Node_ThirdwebCreateWallet.generated.h"

namespace CwPins
{
	extern const FName Type;
	extern const FName Source;
	extern const FName Provider;
	extern const FName AuthInput;
	extern const FName PartnerId;
	extern const FName Wallet;
	extern const FName Success;
	extern const FName Failed;
	extern const FName Error;
	// Smart Wallet Pins
	extern const FName InAppWallet;
	extern const FName SmartWallet;
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
	UEdGraphPin* GetProviderPin() const;
	UEdGraphPin* GetAuthInputPin() const;
	UEdGraphPin* GetWalletPin() const;
	UEdGraphPin* GetErrorPin() const;

	// Smart Wallet Pins
	UEdGraphPin* GetInAppWalletPin() const;
	UEdGraphPin* GetChainIDPin() const;
	UEdGraphPin* GetGaslessPin() const;
	UEdGraphPin* GetFactoryPin() const;
	UEdGraphPin* GetAccountOverridePin() const;
	UEdGraphPin* GetSmartWalletPin() const;

	// Ecosystem Pin
	UEdGraphPin* GetPartnerIDPin() const;

};
