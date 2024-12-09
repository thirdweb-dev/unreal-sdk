// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "HAL/Platform.h"

class FThirdwebMultipartFormData
{
public:
	FThirdwebMultipartFormData()
	{
		Boundary = GenerateBoundary();
	}

	void AddField(const FString& Name, const FString& Value);

	void AddFile(const FString& FieldName, const FString& Filename, const TArray<uint8>& FileContent);
	TArray<uint8> GetContent();

	FString GetBoundary() const { return Boundary; }

private:
	FString Boundary;
	TArray<uint8> FormData;

	static FString GenerateBoundary();

	void AddContent(const FString& Content);
};
