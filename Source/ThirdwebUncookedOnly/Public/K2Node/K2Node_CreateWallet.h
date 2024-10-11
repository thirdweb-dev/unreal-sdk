// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node.h"
#include "K2Node_BaseAsyncTask.h"
#include "ThirdwebLog.h"

#include "GameFramework/GameMode.h"

#include "K2Node_CreateWallet.generated.h"

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
class THIRDWEBUNCOOKEDONLY_API UK2Node_CreateWallet : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()

public:
	// UEdGraphNode interface implementation
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	// End of implementation

	// UK2Node interface implementation
	virtual FText GetMenuCategory() const override;
	virtual void PostReconstructNode() override;
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	// End of implementation

	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

private:
	FNodeTextCache CachedNodeTitle;

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

	UEdGraphPin* GetAuthInputPin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::AuthInput);
		check(Pin == NULL || Pin->Direction == EGPD_Input);
		return Pin;
	}

	UEdGraphPin* GetWalletPin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::Wallet);
		check(Pin == NULL || Pin->Direction == EGPD_Output);
		return Pin;
	}

	UEdGraphPin* GetErrorPin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::Error);
		check(Pin == NULL || Pin->Direction == EGPD_Output);
		return Pin;
	}

	// Smart Wallet Pins
	UEdGraphPin* GetInAppWalletPin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::InAppWallet);
		check(Pin == NULL || Pin->Direction == EGPD_Input);
		return Pin;
	}

	UEdGraphPin* GetChainIDPin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::ChainID);
		check(Pin == NULL || Pin->Direction == EGPD_Input);
		return Pin;
	}

	UEdGraphPin* GetGaslessPin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::Gasless);
		check(Pin == NULL || Pin->Direction == EGPD_Input);
		return Pin;
	}

	UEdGraphPin* GetFactoryPin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::Factory);
		check(Pin == NULL || Pin->Direction == EGPD_Input);
		return Pin;
	}

	UEdGraphPin* GetAccountOverridePin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::AccountOverride);
		check(Pin == NULL || Pin->Direction == EGPD_Input);
		return Pin;
	}

	UEdGraphPin* GetSmartWalletPin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::SmartWallet);
		check(Pin == NULL || Pin->Direction == EGPD_Output);
		return Pin;
	}

	// Ecosystem Pin
	UEdGraphPin* GetPartnerIDPin() const
	{
		UEdGraphPin* Pin = FindPin(CwPins::PartnerId);
		check(Pin == NULL || Pin->Direction == EGPD_Input);
		return Pin;
	}

private:
	static UEdGraphPin* SetPinVisibility(UEdGraphPin* Pin, const bool bShow)
	{
		Pin->bHidden = !bShow;
		return Pin;
	}

	static UEdGraphPin* SetFriendlyName(UEdGraphPin* Pin, const FText& FriendlyName)
	{
		Pin->PinFriendlyName = FriendlyName;
		return Pin;
	}

	static UEdGraphPin* SetNotConnectable(UEdGraphPin* Pin)
	{
		Pin->bNotConnectable = true;
		return Pin;
	}

	static UEdGraphPin* SetAdvancedView(UEdGraphPin* Pin)
	{
		Pin->bAdvancedView = true;
		return Pin;
	}

	void SetHasAdvanced(const bool bHasAdvanced)
	{
		if (bHasAdvanced)
		{
			if (AdvancedPinDisplay == ENodeAdvancedPins::NoPins)
			{
				AdvancedPinDisplay = ENodeAdvancedPins::Hidden;
			}
		}
		else
		{
			AdvancedPinDisplay = ENodeAdvancedPins::NoPins;
		}
	}

	template<typename T = FString>
	static UEdGraphPin* SetDefaultValue(UEdGraphPin* Pin, const T& Value)
	{
		if constexpr (std::is_same_v<T, FString> || std::is_same_v<T, FStringView>)
		{
			Pin->DefaultValue = Value;
		} else if constexpr (std::is_same_v<T, int32>)
		{
			Pin->DefaultValue = FString::Format(TEXT("%d"), Value);
		} else if constexpr (std::is_same_v<T, int64>)
		{
			Pin->DefaultValue = FString::Format(TEXT("%lld"), Value);
		} else if constexpr (std::is_same_v<T, bool>)
		{
			Pin->DefaultValue = static_cast<bool>(Value) ? TEXT("true") : TEXT("false");
		} else
		{
			TW_LOG(Error, TEXT("UK2Node_CreateWallet::SetDefaultValue::Invalid Value Type"));
		}
		return Pin; 
	}
	
	static FString ResolvePinValue(UEdGraphPin* Pin);
	void UpdatePins();

};
