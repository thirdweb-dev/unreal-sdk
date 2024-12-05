// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/BackendWallet/ThirdwebBackendWallet.h"

#include "Dom/JsonObject.h"

FThirdwebBackendWallet FThirdwebBackendWallet::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebBackendWallet Wallet;
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("address")))
		{
			Wallet.Address = JsonObject->GetStringField(TEXT("address"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("type")))
		{
			Wallet.Type = JsonObject->GetStringField(TEXT("type"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("label")))
		{
			Wallet.Label = JsonObject->GetStringField(TEXT("label"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("awsKmsKeyId")))
		{
			Wallet.AwsKmsKeyId = JsonObject->GetStringField(TEXT("awsKmsKeyId"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("awsKmsArn")))
		{
			Wallet.AwsKmsArn = JsonObject->GetStringField(TEXT("awsKmsArn"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("gcpKmsKeyId")))
		{
			Wallet.GcpKmsKeyId = JsonObject->GetStringField(TEXT("gcpKmsKeyId"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("gcpKmsKeyRingId")))
		{
			Wallet.GcpKmsKeyRingId = JsonObject->GetStringField(TEXT("gcpKmsKeyRingId"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("gcpKmsLocationId")))
		{
			Wallet.GcpKmsLocationId = JsonObject->GetStringField(TEXT("gcpKmsLocationId"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("gcpKmsKeyVersionId")))
		{
			Wallet.GcpKmsKeyVersionId = JsonObject->GetStringField(TEXT("gcpKmsKeyVersionId"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("gcpKmsResourcePath")))
		{
			Wallet.GcpKmsResourcePath = JsonObject->GetStringField(TEXT("gcpKmsResourcePath"));
		}
	}
	return Wallet;
}

TArray<FThirdwebBackendWallet> FThirdwebBackendWallet::FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray)
{
	TArray<FThirdwebBackendWallet> Wallets;
	for (TSharedPtr JsonObject : JsonArray)
	{
		if (JsonObject.IsValid() && JsonObject->Type == EJson::Object)
		{
			Wallets.Emplace(FromJson(JsonObject->AsObject()));
		}
	}

	return Wallets;
}
