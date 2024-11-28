// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_ThirdwebFetchIpfsData.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "ThirdwebRuntimeSettings.h"
#include "TWUOCommon.h"
#include "TWUOUtils.h"
#include "AsyncTasks/Utils/AsyncTaskThirdwebFetchIpfsData.h"
#include "Engine/Texture2DDynamic.h"
#include "Styling/SlateIconFinder.h"
#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

namespace TwPins
{
	const FName Uri = FName(TEXT("Uri"));
	const FName Data = FName(TEXT("Data"));
}

#define LOCTEXT_NAMESPACE "ThirdwebUncookedOnly"

UK2Node_ThirdwebFetchIpfsData::UK2Node_ThirdwebFetchIpfsData()
{
	ProxyClass = UAsyncTaskThirdwebFetchIpfsJson::StaticClass();
	ProxyFactoryClass = UAsyncTaskThirdwebFetchIpfsJson::StaticClass();
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebFetchIpfsJson, FetchIpfsJson);
}

FText UK2Node_ThirdwebFetchIpfsData::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::MenuTitle)
	{
		return LOCTEXT("K2Node_ThirdwebFetchIpfsData_SmartNodeTitle", "Fetch IPFS Data");
	}
	if (CachedNodeTitle.IsOutOfDate(this))
	{
		CachedNodeTitle.MarkDirty();
		if (UEdGraphPin* Pin = GetDataPin())
		{
			if (Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_String)
			{
				CachedNodeTitle.SetCachedText(LOCTEXT("K2Node_ThirdwebFetchIpfsData_SmartNodeTitle", "Fetch IPFS JSON Data"), this);
			}
			else if (Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Object)
			{
				CachedNodeTitle.SetCachedText(LOCTEXT("K2Node_ThirdwebFetchIpfsData_SmartNodeTitle", "Fetch IPFS Image Data"), this);
			}
			else if (Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Byte)
			{
				CachedNodeTitle.SetCachedText(LOCTEXT("K2Node_ThirdwebFetchIpfsData_SmartNodeTitle", "Fetch IPFS Raw Data"), this);
			}
			else
			{
				CachedNodeTitle.SetCachedText(LOCTEXT("K2Node_ThirdwebFetchIpfsData_SmartNodeTitle", "Fetch IPFS Data"), this);
			}
		}
	}
	return CachedNodeTitle;
}

FText UK2Node_ThirdwebFetchIpfsData::GetTooltipText() const
{
	return LOCTEXT("K2Node_ThirdwebFetchIpfsData_TooltipText", "Fetch Data from an IPFS URL");
}

void UK2Node_ThirdwebFetchIpfsData::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::Super::PinDefaultValueChanged(Pin);
	if (Pin == GetDataPin())
	{
		CachedNodeTitle.MarkDirty();
	}
	UpdatePins();
}

bool UK2Node_ThirdwebFetchIpfsData::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const
{
	if (MyPin == GetDataPin() && OtherPin)
	{
		if (OtherPin->PinType.PinCategory != UEdGraphSchema_K2::PC_String && OtherPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Object && OtherPin->PinType.PinCategory !=
			UEdGraphSchema_K2::PC_Byte)
		{
			OutReason = LOCTEXT("K2Node_ThirdwebFetchIpfsData_IsConnectionDisallowed", "Only String, Texture2DDynamic, and Byte Arrays allowed").ToString();
			return true;
		}
		if ((OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_String || OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Object) && OtherPin->PinType.ContainerType !=
			EPinContainerType::None)
		{
			OutReason = LOCTEXT("K2Node_ThirdwebFetchIpfsData_IsConnectionDisallowed", "Only Single Values Allowed").ToString();
			return true;
		}
		if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Byte && OtherPin->PinType.ContainerType != EPinContainerType::Array)
		{
			OutReason = LOCTEXT("K2Node_ThirdwebFetchIpfsData_IsConnectionDisallowed", "Bytes must be an array").ToString();
			return true;
		}
		if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Object && OtherPin->PinType.PinSubCategoryObject != UTexture2DDynamic::StaticClass())
		{
			OutReason = LOCTEXT("K2Node_ThirdwebFetchIpfsData_IsConnectionDisallowed", "Image must be Texture2DDynamic").ToString();
			return true;
		}
	}
	return Super::IsConnectionDisallowed(MyPin, OtherPin, OutReason);
}

void UK2Node_ThirdwebFetchIpfsData::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	SetPinFriendlyName(
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Uri),
		LOCTEXT("K2Node_ThirdwebFetchIpfsData_Uri", "URI")
	);

	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, TwPins::Data);

	PostAllocateDefaultPins();
}

void UK2Node_ThirdwebFetchIpfsData::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	if (UEdGraphPin* Pin = GetDataPin())
	{
		if (Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Object)
		{
			ProxyClass = UAsyncTaskThirdwebFetchIpfsImage::StaticClass();
			ProxyFactoryClass = UAsyncTaskThirdwebFetchIpfsImage::StaticClass();
			ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebFetchIpfsImage, FetchIpfsImage);
		}
		else if (Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Byte)
		{
			ProxyClass = UAsyncTaskThirdwebFetchIpfsRaw::StaticClass();
			ProxyFactoryClass = UAsyncTaskThirdwebFetchIpfsRaw::StaticClass();
			ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebFetchIpfsRaw, FetchIpfsRaw);
		}
		else
		{
			ProxyClass = UAsyncTaskThirdwebFetchIpfsJson::StaticClass();
			ProxyFactoryClass = UAsyncTaskThirdwebFetchIpfsJson::StaticClass();
			ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskThirdwebFetchIpfsJson, FetchIpfsJson);
		}
	}
	Super::ExpandNode(CompilerContext, SourceGraph);
}

void UK2Node_ThirdwebFetchIpfsData::UpdatePins()
{
	if (UEdGraphPin* Pin = GetDataPin())
	{
		if (const UEdGraphPin* ConnectedPin = GetConnectedPin(Pin))
		{
			if (Pin->PinType != ConnectedPin->PinType || Pin->PinType.ContainerType != ConnectedPin->PinType.ContainerType)
			{
				Pin->PinType = ConnectedPin->PinType;
				Pin->PinType.ContainerType = ConnectedPin->PinType.ContainerType;
				if (Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Object)
				{
					SetPinFriendlyName(Pin, LOCTEXT("K2Node_ThirdwebFetchIpfsData_Texture2DDynamicData", "Image"));
				}
				else if (Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Byte)
				{
					SetPinFriendlyName(Pin, LOCTEXT("K2Node_ThirdwebFetchIpfsData_RawData", "Raw Data"));
				}
				else if (Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_String)
				{
					SetPinFriendlyName(Pin, LOCTEXT("K2Node_ThirdwebFetchIpfsData_JsonData", "JSON String"));
				}
				CachedNodeTitle.MarkDirty();
				GetGraph()->NotifyGraphChanged();
			}
		}
		else
		{
			if (Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard || Pin->PinType.ContainerType != EPinContainerType::None)
			{
				Pin->PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
				Pin->PinType.ContainerType = EPinContainerType::None;
				Pin->BreakAllPinLinks();
				SetPinFriendlyName(Pin, LOCTEXT("K2Node_ThirdwebFetchIpfsData_JsonData", "Data"));
				CachedNodeTitle.MarkDirty();
				GetGraph()->NotifyGraphChanged();
			}
		}
	}
}

UEdGraphPin* UK2Node_ThirdwebFetchIpfsData::GetUriPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::Uri);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ThirdwebFetchIpfsData::GetDataPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::Data);
	check(Pin == NULL || Pin->Direction == EGPD_Output);
	return Pin;
}

#undef LOCTEXT_NAMESPACE
