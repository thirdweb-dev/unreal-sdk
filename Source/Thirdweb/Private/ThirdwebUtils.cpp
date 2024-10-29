// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebUtils.h"

#include "Thirdweb.h"
#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"

#include "Dom/JsonObject.h"

#include "GenericPlatform/GenericPlatformHttp.h"

#include "Interfaces/IHttpRequest.h"

#include "Kismet/KismetStringLibrary.h"

#include "Policies/CondensedJsonPrintPolicy.h"

#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"

#define LOCTEXT_NAMESPACE "Thirdweb"

namespace ThirdwebUtils
{
	const TCHAR* ZeroAddress = TEXT("0x0000000000000000000000000000000000000000");

	bool IsChecksummedAddress(const FString& Address) { return Thirdweb::is_valid_address(TO_RUST_STRING(Address), true).GetOutput().ToBool(); }

	bool IsValidAddress(const FString& Address, const bool bWithChecksum) { return Thirdweb::is_valid_address(TO_RUST_STRING(Address), bWithChecksum).GetOutput().ToBool(); }

	FString ToChecksummedAddress(const FString& Address) { return Thirdweb::to_checksummed_address(TO_RUST_STRING(Address)).GetOutput(); }

	FText ToText(const EThirdwebOAuthProvider Provider) { return Maps::OAuthProviderToText.Contains(Provider) ? Maps::OAuthProviderToText[Provider] : FText::FromString(TEXT("Invalid")); }

	FString ToString(const EThirdwebOAuthProvider Provider) { return ToText(Provider).ToString(); }

	EThirdwebOAuthProvider ToOAuthProvider(const FText& Text)
	{
		for (const auto& It : Maps::OAuthProviderToText)
		{
			if (It.Value.EqualToCaseIgnored(Text))
			{
				return It.Key;
			}
		}
		return EThirdwebOAuthProvider::None;
	}

	EThirdwebOAuthProvider ToOAuthProvider(const FString& String) { return ToOAuthProvider(FText::FromString(String)); }

	FString ParseAuthResult(const FString& AuthResult)
	{
		FString Result = AuthResult;
		TW_LOG(Verbose, TEXT("ThirdwebUtils::ParseAuthResult::Initial::%s"), *Result)
		if (Result.StartsWith(TEXT("%7B%22")))
		{
			Result = FGenericPlatformHttp::UrlDecode(Result);
			TW_LOG(VeryVerbose, TEXT("ThirdwebUtils::ParseAuthResult::UrlDecoded::%s"), *Result)
		}
		return Result;
	}

	namespace Internal
	{
		FString MaskSensitiveString(const FString& InString, const FString& MatchString, const FString& MaskCharacter, const int32 ShowBeginCount, const int32 ShowEndCount)
		{
			FString UnmatchedLeft, UnmatchedRight;
			if (InString.Split(MatchString, &UnmatchedLeft, &UnmatchedRight))
			{
				return UnmatchedLeft + MatchString.Left(ShowBeginCount) + MaskCharacter + MaskCharacter + MaskCharacter + MatchString.Right(ShowEndCount) + UnmatchedRight;
			}
			return InString;
		}

		TArray<FString> MaskSensitiveString(const TArray<FString>& InStrings, const FString& MatchString, const FString& MaskCharacter, const int32 ShowBeginCount, const int32 ShowEndCount)
		{
			TArray<FString> Result;
			for (const FString& InString : InStrings)
			{
				Result.Emplace(MaskSensitiveString(InString, MatchString, MaskCharacter, ShowBeginCount, ShowEndCount));
			}
			return Result;
		}

		TArray<FString> MaskSensitiveString(const TArray<FString>& InStrings, const TArray<FString>& MatchStrings, const FString& MaskCharacter, const int32 ShowBeginCount, const int32 ShowEndCount)
		{
			TArray<FString> Result;
			for (const FString& InString : InStrings)
			{
				FString ResultString = InString;
				for (const FString& MatchString : MatchStrings)
				{
					ResultString = MaskSensitiveString(ResultString, MatchString, MaskCharacter, ShowBeginCount, ShowEndCount);
				}
				Result.Emplace(ResultString);
			}
			return Result;
		}

		void LogRequest(const TSharedRef<IHttpRequest>& Request, const TArray<FString>& SensitiveStrings)
		{
			TArray<uint8> Content = Request->GetContent();
			TW_LOG(
				VeryVerbose,
				TEXT("ThirdwebUtils::Internal::LogRequest::URL=%s | Headers=%s | Content=%s"),
				*Request->GetURL(),
				*UKismetStringLibrary::JoinStringArray(ThirdwebUtils::Internal::MaskSensitiveString(Request->GetAllHeaders(), SensitiveStrings), TEXT(";")),
				ANSI_TO_TCHAR(reinterpret_cast<const char*>(Request->GetContent().GetData()))
			)
		}
	}

	namespace Maps
	{
		const TMap<EThirdwebOAuthProvider, FText> OAuthProviderToText = {
			{EThirdwebOAuthProvider::Google, LOCTEXT("Google", "Google")},
			{EThirdwebOAuthProvider::Apple, LOCTEXT("Apple", "Apple")},
			{EThirdwebOAuthProvider::Facebook, LOCTEXT("Facebook", "Facebook")},
			{EThirdwebOAuthProvider::Discord, LOCTEXT("Discord", "Discord")},
			{EThirdwebOAuthProvider::Farcaster, LOCTEXT("Farcaster", "Farcaster")},
			{EThirdwebOAuthProvider::Telegram, LOCTEXT("Telegram", "Telegram")},
			{EThirdwebOAuthProvider::Line, LOCTEXT("Line", "Line")},
			{EThirdwebOAuthProvider::X, LOCTEXT("X", "X")},
			{EThirdwebOAuthProvider::Coinbase, LOCTEXT("Coinbase", "Coinbase")},
			{EThirdwebOAuthProvider::Twitch, LOCTEXT("Twitch", "Twitch")},
			{EThirdwebOAuthProvider::Github, LOCTEXT("Github", "Github")}
		};
	}

	namespace Json
	{
		TSharedPtr<FJsonObject> ToJson(const FString& String)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
			// ReSharper disable once CppTooWideScopeInitStatement
			const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(String);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				return JsonObject;
			}
			return TSharedPtr<FJsonObject>();
		}

		TArray<TSharedPtr<FJsonValue>> ToJsonArray(const FString& String)
		{
			TArray<TSharedPtr<FJsonValue>> JsonArray;
			const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(String);
			FJsonSerializer::Deserialize(Reader, JsonArray);
			return JsonArray;
		}

		FString ToString(const TSharedPtr<FJsonObject>& JsonObject)
		{
			FString Out;
			const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Out);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			return Out;
		}
	}
}

#undef LOCTEXT_NAMESPACE
