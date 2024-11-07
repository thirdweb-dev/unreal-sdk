// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "IImageWrapper.h"
#include "ThirdwebEditorCommon.generated.h"

UENUM()
enum class EThirdwebEditorBlueprintImageFormat : uint8
{
	PNG = static_cast<uint8>(EImageFormat::PNG),
	JPEG = static_cast<uint8>(EImageFormat::JPEG),
	BMP = static_cast<uint8>(EImageFormat::BMP),
};

struct FThirdwebEditorScreenshotData
{
	FThirdwebEditorScreenshotData()
	{
		Size = FIntVector();
	}
	
	FThirdwebEditorScreenshotData(const FVector2D& InSize)
	{
		Size = FIntVector(InSize.X, InSize.Y, 0);
	}

	TArray<FColor> ColorData;
	FIntVector Size;
	FString CustomName;

	FIntPoint GetPointSize() const { return FIntPoint(Size.X, Size.Y); }
	bool IsValid() const { return ColorData.Num() > 0 && Size.X > 0 && Size.Y > 0; }
};
