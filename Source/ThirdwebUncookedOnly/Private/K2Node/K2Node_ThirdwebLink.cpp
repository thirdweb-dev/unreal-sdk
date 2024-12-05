// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_ThirdwebLink.h"

#include "TWUOUtils.h"
#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebLink.h"

namespace TwPins
{
	const FName NewWallet = FName(TEXT("NewWallet"));
}

#define LOCTEXT_NAMESPACE "ThirdwebUncookedOnly"

UK2Node_ThirdwebLink::UK2Node_ThirdwebLink()
{
	Source = EThirdwebInAppWalletSource::OAuth;
	ProxyClass = UAsyncTaskThirdwebLink::StaticClass();
	ProxyFactoryClass = UAsyncTaskThirdwebLink::StaticClass();
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebLink, Link);
}

#if WITH_EDITOR
void UK2Node_ThirdwebLink::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UK2Node_ThirdwebLink, Source))
	{
		if (UpdatePins())
		{
			ReconstructNode();
		}
	}
}
#endif

FText UK2Node_ThirdwebLink::GetNodeTitle(const ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::MenuTitle)
	{
		return LOCTEXT("K2Node_ThirdwebLink_MenuNodeTitle", "Link Wallet");
	}
	if (CachedNodeTitle.IsOutOfDate(this))
	{
		CachedNodeTitle.SetCachedText(FText::Format(LOCTEXT("K2Node_ThirdwebLink_GraphNodeTitle", "Link {0} Wallet"), ThirdwebUtils::ToText(Source)), this);
	}
	return CachedNodeTitle;
}

FText UK2Node_ThirdwebLink::GetTooltipText() const
{
	return LOCTEXT("K2Node_ThirdwebLink_TooltipText", "Link a Thirdweb Wallet");
}

FText UK2Node_ThirdwebLink::GetMenuCategory() const
{
	return LOCTEXT("K2Node_ThirdwebLink_Category", "Thirdweb|Wallets|InApp");
}

void UK2Node_ThirdwebLink::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::Super::PinDefaultValueChanged(Pin);
	UpdatePins();
}

void UK2Node_ThirdwebLink::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	
	// Input Pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), TwPins::Wallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), TwPins::NewWallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Input);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Signature);
	PostAllocateDefaultPins();
}

bool UK2Node_ThirdwebLink::UpdatePins()
{
	bool bUpdate = false;
	if (UEdGraphPin* SignaturePin = GetSignaturePin())
	{
		if (Source != EThirdwebInAppWalletSource::Siwe && SignaturePin->LinkedTo.Num() > 0)
		{
			SignaturePin->BreakAllPinLinks();
			bUpdate = true;
		}
		if (bool bShow = Source == EThirdwebInAppWalletSource::Siwe; bShow != !SignaturePin->bHidden)
		{
			SetPinVisibility(GetSignaturePin(), bShow);
			bUpdate = true;
		}
	}
	if (UEdGraphPin* InputPin = GetInputPin())
	{
		if (Source == EThirdwebInAppWalletSource::Guest && InputPin->LinkedTo.Num() > 0)
		{
			InputPin->BreakAllPinLinks();
			bUpdate = true;
		}
		if (bool bShow = Source != EThirdwebInAppWalletSource::Guest; bShow != !InputPin->bHidden)
		{
			SetPinVisibility(GetInputPin(), bShow);
			bUpdate = true;
		}
		static FText OTPText = LOCTEXT("UK2Node_ThirdwebLink_OTPInput", "OTP");
		if ((Source == EThirdwebInAppWalletSource::Phone || Source == EThirdwebInAppWalletSource::Email) && !InputPin->PinFriendlyName.EqualTo(OTPText))
		{
			SetPinFriendlyName(InputPin, OTPText);
			bUpdate = true;
		}
		static FText OAuthText = LOCTEXT("UK2Node_ThirdwebLink_AuthResultInput", "Auth Result");
		if (Source == EThirdwebInAppWalletSource::OAuth && !InputPin->PinFriendlyName.EqualTo(OAuthText))
		{
			SetPinFriendlyName(InputPin, OAuthText);
			bUpdate = true;
		}
		TArray PayloadTypes = {EThirdwebInAppWalletSource::Jwt, EThirdwebInAppWalletSource::Siwe, EThirdwebInAppWalletSource::AuthEndpoint};
		static FText PayloadText = LOCTEXT("UK2Node_ThirdwebLink_PayloadInput", "Payload");
		if (PayloadTypes.Contains(Source) && !InputPin->PinFriendlyName.EqualTo(PayloadText))
		{
			SetPinFriendlyName(InputPin, PayloadText);
			bUpdate = true;
		}
	}

	if (bUpdate)
	{
		CachedNodeTitle.MarkDirty();
		GetGraph()->NotifyGraphChanged();
	}
	return bUpdate;
}

#undef LOCTEXT_NAMESPACE
