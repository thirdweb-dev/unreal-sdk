// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "Interfaces/IHttpRequest.h"
#include "Templates/SharedPointer.h"
#include "Templates/Tuple.h"

class FThirdwebHeaders
{
public:
	// Constructor
	FThirdwebHeaders();

	// Adds a header conditionally
	void Set(const FString& Name, const FString& Value, const bool bCondition = true);

	// Adds headers
	void SetMany(TArray<TTuple<FString, FString>> Pairs);
	
	// Removes a header
	bool Remove(const FString& Name);

	// Retrieves a header value
	FString Get(const FString& Name) const;

	// Retrieves all headers
	const TMap<FString, FString>& GetAll() const;

	void UpdateRequest(const TSharedPtr<IHttpRequest>& Request);
private:
	// Map to store header key-value pairs
	TMap<FString, FString> Headers;
};
