// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TWUOFunctionLibrary.generated.h"

enum class EThirdwebInAppWalletSource : uint8;
struct FThirdwebEngineTransactionStatusResult;
struct FThirdwebCountryCode;
struct FInAppWalletHandle;
struct FSmartWalletHandle;
enum class EFunctionResult : uint8;
enum class EThirdwebOAuthProvider : uint8;

/**
 * Thirdweb Function Library
 */
UCLASS(meta=(BlueprintThreadSafe, DisplayName="Thirdweb", ScriptName="TWUOFunctionLibrary"))
class THIRDWEBUNCOOKEDONLY_API UTWUOFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, DisplayName="To String (In App Wallet Source)", meta=(CompactNodeTitle="=>", BlueprintAutocast), Category="Utilities|String|In App Wallet")
	static FString Conv_ThirdwebInAppWalletSourceToString(EThirdwebInAppWalletSource Source);

	UFUNCTION(BlueprintPure, DisplayName="To In App Wallet Source", meta=(CompactNodeTitle="=>", BlueprintAutocast), Category="Utilities|String|In App Wallet")
	static EThirdwebInAppWalletSource Conv_StringToThirdwebInAppWalletSource(FString Source);

};
