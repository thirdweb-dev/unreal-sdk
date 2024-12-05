// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node.h"
#include "K2Node_ThirdwebBaseAsyncTask.h"
#include "TWUOCommon.h"
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
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	// End of implementation
	
	// UK2Node interface implementation
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual bool ShouldShowNodeProperties() const override { return true; }
	// End of implementation

protected:
	UPROPERTY(EditDefaultsOnly, Category="Config")
	EThirdwebWalletType Type;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="Type==EThirdwebWalletType::InApp", EditConditionHides), Category="Config")
	EThirdwebInAppWalletSource Source;

protected:
	virtual bool UpdatePins() override;
	
	virtual UEdGraphPin* GetWalletPin() const override;
	
	// Smart Wallet Pins
	UEdGraphPin* GetChainIDPin() const;
	UEdGraphPin* GetGaslessPin() const;
	UEdGraphPin* GetFactoryPin() const;
	UEdGraphPin* GetAccountOverridePin() const;
	virtual UEdGraphPin* GetSmartWalletPin() const override;
};
