// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Containers/ThirdwebLinkedAccount.h"

FThirdwebLinkedAccount FThirdwebLinkedAccount::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebLinkedAccount LinkedAccount;
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("type")))
		{
			LinkedAccount.Type = JsonObject->GetStringField(TEXT("type"));
		}
		if (JsonObject->HasTypedField<EJson::Object>(TEXT("details")))
		{
			const TSharedPtr<FJsonObject> DetailsJsonObject = JsonObject->GetObjectField(TEXT("details"));
			if (DetailsJsonObject->HasTypedField<EJson::String>(TEXT("id")))
			{
				LinkedAccount.Id = FName(*DetailsJsonObject->GetStringField(TEXT("id")));
			}
			if (!DetailsJsonObject->HasTypedField<EJson::Null>(TEXT("email")) && DetailsJsonObject->HasTypedField<EJson::String>(TEXT("email")))
			{
				LinkedAccount.Email = DetailsJsonObject->GetStringField(TEXT("email"));
			}
			if (!DetailsJsonObject->HasTypedField<EJson::Null>(TEXT("address")) && DetailsJsonObject->HasTypedField<EJson::String>(TEXT("address")))
			{
				LinkedAccount.Address = DetailsJsonObject->GetStringField(TEXT("address"));
			}
			if (!DetailsJsonObject->HasTypedField<EJson::Null>(TEXT("phone")) && DetailsJsonObject->HasTypedField<EJson::String>(TEXT("phone")))
			{
				LinkedAccount.Phone = DetailsJsonObject->GetStringField(TEXT("phone"));
			}
		}
	}
	return LinkedAccount;
}

