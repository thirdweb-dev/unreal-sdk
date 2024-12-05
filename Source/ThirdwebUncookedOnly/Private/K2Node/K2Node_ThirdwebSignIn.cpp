// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_ThirdwebSignIn.h"

#include "TWUOUtils.h"
#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebSignIn.h"

#define LOCTEXT_NAMESPACE "ThirdwebUncookedOnly"

UK2Node_ThirdwebSignIn::UK2Node_ThirdwebSignIn()
{
	Source = EThirdwebInAppWalletSource::OAuth;
	ProxyClass = UAsyncTaskThirdwebSignIn::StaticClass();
	ProxyFactoryClass = UAsyncTaskThirdwebSignIn::StaticClass();
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebSignIn, SignIn);
}

#if WITH_EDITOR
void UK2Node_ThirdwebSignIn::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UK2Node_ThirdwebSignIn, Source))
	{
		if (UpdatePins())
		{
			ReconstructNode();
		}
	}
}
#endif

FText UK2Node_ThirdwebSignIn::GetNodeTitle(const ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::MenuTitle)
	{
		return LOCTEXT("K2Node_ThirdwebSignIn_MenuNodeTitle", "Sign In");
	}
	if (CachedNodeTitle.IsOutOfDate(this))
	{
		CachedNodeTitle.SetCachedText(
			FText::Format(
				LOCTEXT("K2Node_ThirdwebSignIn_GraphNodeTitle", "Sign In with {0}"),
				Source == EThirdwebInAppWalletSource::Siwe ? FText::FromString(TEXT("Ethereum")) : ThirdwebUtils::ToText(Source)
			),
			this
		);
	}
	return CachedNodeTitle;
}

FText UK2Node_ThirdwebSignIn::GetTooltipText() const
{
	return LOCTEXT("K2Node_ThirdwebSignIn_TooltipText", "Sign In With a Thirdweb Wallet");
}

FText UK2Node_ThirdwebSignIn::GetMenuCategory() const
{
	return LOCTEXT("K2Node_ThirdwebSignIn_Category", "Thirdweb|Wallets|InApp");
}

bool UK2Node_ThirdwebSignIn::UpdatePins()
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
		static FText OTPText = LOCTEXT("UK2Node_ThirdwebSignIn_OTPInput", "OTP");
		if ((Source == EThirdwebInAppWalletSource::Phone || Source == EThirdwebInAppWalletSource::Email) && !InputPin->PinFriendlyName.EqualTo(OTPText))
		{
			SetPinFriendlyName(InputPin, OTPText);
			bUpdate = true;
		}
		static FText OAuthText = LOCTEXT("UK2Node_ThirdwebSignIn_AuthResultInput", "Auth Result");
		if (Source == EThirdwebInAppWalletSource::OAuth && !InputPin->PinFriendlyName.EqualTo(OAuthText))
		{
			SetPinFriendlyName(InputPin, OAuthText);
			bUpdate = true;
		}
		TArray PayloadTypes = {EThirdwebInAppWalletSource::Jwt, EThirdwebInAppWalletSource::Siwe, EThirdwebInAppWalletSource::AuthEndpoint};
		static FText PayloadText = LOCTEXT("UK2Node_ThirdwebSignIn_PayloadInput", "Payload");
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

void UK2Node_ThirdwebSignIn::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::Super::PinDefaultValueChanged(Pin);
	UpdatePins();
}

void UK2Node_ThirdwebSignIn::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	// Input Pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FInAppWalletHandle::StaticStruct(), TwPins::Wallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Input);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Signature);
	PostAllocateDefaultPins();
}

#undef LOCTEXT_NAMESPACE
