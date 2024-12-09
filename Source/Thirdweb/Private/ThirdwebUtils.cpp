// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebUtils.h"

#include <random>

#include "HttpModule.h"
#include "IImageWrapperModule.h"
#include "RenderingThread.h"
#include "TextureResource.h"
#include "Thirdweb.h"
#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
#include "Async/TaskGraphInterfaces.h"
#include "Containers/ThirdwebIPFSUploadResult.h"
#include "Containers/ThirdwebMultipartFormData.h"
#include "Dom/JsonObject.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Engine/ThirdwebEngineCommon.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IPluginManager.h"
#include "Internal/ThirdwebHeaders.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/Base64.h"
#include "Misc/DefaultValueHelper.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "ThirdParty/QRCodeGenerator.h"
#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"
#include "Wallets/ThirdwebWalletHandle.h"

#define LOCTEXT_NAMESPACE "Thirdweb"

namespace ThirdwebUtils
{
	const TCHAR* ZeroAddress = TEXT("0x0000000000000000000000000000000000000000");

	bool IsChecksummedAddress(const FString& Address) { return Thirdweb::is_valid_address(TO_RUST_STRING(Address), true).GetOutput().ToBool(); }

	bool IsValidAddress(const FString& Address, const bool bWithChecksum) { return Thirdweb::is_valid_address(TO_RUST_STRING(Address), bWithChecksum).GetOutput().ToBool(); }

	FString ToChecksummedAddress(const FString& Address) { return Thirdweb::to_checksummed_address(TO_RUST_STRING(Address)).GetOutput(); }

	FText ToText(const EThirdwebOAuthProvider Provider) { return Maps::OAuthProviderToText.Contains(Provider) ? Maps::OAuthProviderToText[Provider] : FText::FromString(TEXT("Invalid")); }

	FText ToText(const EThirdwebEngineTransactionStatus Status) { return Maps::TransactionStatusToText.Contains(Status) ? Maps::TransactionStatusToText[Status] : FText::FromString(TEXT("Invalid")); }

	FText ToText(const EThirdwebEngineTransactionOnChainStatus Status) { return Maps::OnChainStatusToText.Contains(Status) ? Maps::OnChainStatusToText[Status] : FText::FromString(TEXT("Invalid")); }

	FString ToString(const EThirdwebOAuthProvider Provider) { return ToText(Provider).ToString(); }

	FString ToString(const EThirdwebEngineTransactionStatus Status) { return ToText(Status).ToString(); }

	FString ToString(const EThirdwebEngineTransactionOnChainStatus Status) { return ToText(Status).ToString(); }

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

	EThirdwebEngineTransactionStatus ToTransactionStatus(const FText& Text)
	{
		for (const auto& It : Maps::TransactionStatusToText)
		{
			if (It.Value.EqualToCaseIgnored(Text))
			{
				return It.Key;
			}
		}
		return EThirdwebEngineTransactionStatus::Unknown;
	}

	EThirdwebEngineTransactionOnChainStatus ToOnChainStatus(const FText& Text)
	{
		for (const auto& It : Maps::OnChainStatusToText)
		{
			if (It.Value.EqualToCaseIgnored(Text))
			{
				return It.Key;
			}
		}
		return EThirdwebEngineTransactionOnChainStatus::Unknown;
	}

	EThirdwebOAuthProvider ToOAuthProvider(const FString& String) { return ToOAuthProvider(FText::FromString(String)); }

	EThirdwebEngineTransactionStatus ToTransactionStatus(const FString& String) { return ToTransactionStatus(FText::FromString(String)); }

	EThirdwebEngineTransactionOnChainStatus ToOnChainStatus(const FString& String) { return ToOnChainStatus(FText::FromString(String)); }

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

	UTexture2D* CreateQrCode(const FString& Text)
	{
		QRCodeGenerator::FQRCode QrCode = QRCodeGenerator::FQRCode::EncodeText(TCHAR_TO_UTF8(*Text), QRCodeGenerator::FQRCode::ECC::Low);

		uint8 Size = QrCode.GetSize();
		TArray<FColor> Pixels;
		Pixels.SetNumZeroed(Size * Size);

		for (uint8 x = 0; x < Size; x++)
		{
			for (uint8 y = 0; y < Size; y++)
			{
				Pixels[x + y * Size] = QrCode.GetModule(x, y) ? FColor::White : FColor::Black;
			}
		}

		UTexture2D* Texture = UTexture2D::CreateTransient(Size, Size, PF_B8G8R8A8, "QRCode");
		void* Data = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(Data, Pixels.GetData(), Size * Size * 4);
		Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
		Texture->UpdateResource();
		Texture->Filter = TF_Nearest;

		return Texture;
	}

	namespace Storage
	{
		void Download(const FString& Url, const FDownloadBytesSuccessDelegate& Success, const FStringDelegate& Error)
		{
			{
				FString FinalUrl = Url;
				if (FinalUrl.IsEmpty())
				{
					EXECUTE_IF_BOUND(Error, TEXT("Empty URL"))
				}
				static const TCHAR* Base64Prefix = TEXT("data:application/json;base64,");
				if (FinalUrl.StartsWith(Base64Prefix))
				{
					FBase64::Decode(FinalUrl.Replace(Base64Prefix, TEXT("")), FinalUrl);
					EXECUTE_IF_BOUND(Success, Internal::StringToBytes(FinalUrl));
					return;
				}
				FinalUrl = Internal::ReplaceIpfs(FinalUrl, FString::Printf(TEXT("https://%s.ipfscdn.io/ipfs/"), *UThirdwebRuntimeSettings::GetClientId()));

				FHttpModule& HttpModule = FHttpModule::Get();
				const TSharedRef<IHttpRequest> Request = HttpModule.CreateRequest();
				Request->SetVerb(TEXT("GET"));
				Request->SetTimeout(30.0f);
				Request->SetURL(FinalUrl);
				Request->OnProcessRequestComplete().BindLambda([Success, Error](FHttpRequestPtr, const FHttpResponsePtr& Response, const bool bConnectedSuccessfully)
				{
					if (bConnectedSuccessfully)
					{
						if (Response.IsValid())
						{
							EXECUTE_IF_BOUND(Success, Response->GetContent())
						}
						else
						{
							EXECUTE_IF_BOUND(Error, TEXT("Invalid Response Object"))
						}
					}
					else
					{
						EXECUTE_IF_BOUND(Error, TEXT("Network Connection Error"))
					}
				});
				Request->ProcessRequest();
			}
		}

		template <typename T>
		T ConvertDownloadResult(const TArray<uint8>& Bytes)
		{
			if constexpr (std::is_same_v<T, TSharedPtr<FJsonObject>>)
			{
				return Json::ToJson(Internal::BytesToString(Bytes));
			}
			else if constexpr (std::is_same_v<T, UTexture2DDynamic*>)
			{
				return Internal::BytesToTexture2DDynamic(Bytes);
			}
			else if constexpr (std::is_same_v<T, FString>)
			{
				return Internal::BytesToString(Bytes);
			}
			else
			{
				static_assert(std::is_same_v<T, TSharedPtr<FJsonObject>> || std::is_same_v<T, UTexture2DDynamic*> || std::is_same_v<T, FString>, "Unsupported type for ConvertDownloadResult");
				return T{};
			}
		}

		template THIRDWEB_API TSharedPtr<FJsonObject> ConvertDownloadResult<TSharedPtr<FJsonObject>>(const TArray<uint8>& Bytes);
		template THIRDWEB_API UTexture2DDynamic* ConvertDownloadResult<UTexture2DDynamic*>(const TArray<uint8>& Bytes);
		template THIRDWEB_API FString ConvertDownloadResult<FString>(const TArray<uint8>& Bytes);

		template <typename T>
		void Upload(const FString& Filename, const T Content, const FUploadSuccessDelegate& Success, const FStringDelegate& Error)

		{
			if constexpr (std::is_same_v<T, TArray<uint8>>)
			{
				return UploadInternal(Filename, Content, Success, Error);
			}
			else if constexpr (std::is_same_v<T, FString>)
			{
				return UploadInternal(Filename, Internal::StringToBytes(Content), Success, Error);
			}
			else
			{
				static_assert(std::is_same_v<T, TArray<uint8>> || std::is_same_v<T, FString>, "Unsupported type for Upload");
			}
		}

		template THIRDWEB_API void Upload<TArray<uint8>>(const FString& Filename, const TArray<uint8> Content, const FUploadSuccessDelegate& Success, const FStringDelegate& Error);
		template THIRDWEB_API void Upload<FString>(const FString& Filename, const FString Content, const FUploadSuccessDelegate& Success, const FStringDelegate& Error);

		void UploadInternal(const FString& Filename, const TArray<uint8>& Content, const FUploadSuccessDelegate& Success, const FStringDelegate& Error)
		{
			if (Content.Num() == 0)
			{
				EXECUTE_IF_BOUND(Error, TEXT("Empty Content"))
				return;
			}
			FHttpModule& HttpModule = FHttpModule::Get();
			const TSharedRef<IHttpRequest> Request = HttpModule.CreateRequest();
			Request->SetVerb(TEXT("POST"));

			// Prepare multipart form data
			FThirdwebMultipartFormData FormData;
			FormData.AddFile(TEXT("file"), Filename, Content);

			// Get the content and boundary
			TArray<uint8> RequestContent = FormData.GetContent();
			FString Boundary = FormData.GetBoundary();

			// Set the request content
			Request->SetContent(RequestContent);

			// Set the content type to multipart/form-data
			Request->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=") + Boundary);

			// Bind the request completion callbacks
			Request->OnProcessRequestComplete().BindLambda([Success, Error](FHttpRequestPtr, const FHttpResponsePtr& Response, const bool bConnectedSuccessfully)
			{
				if (bConnectedSuccessfully && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
				{
					EXECUTE_IF_BOUND(Success, FThirdwebIPFSUploadResult::FromJson(Response->GetContentAsString()));
				}
				else
				{
					EXECUTE_IF_BOUND(Error, Response.IsValid() ? Response->GetContentAsString() : TEXT("Request failed"));
				}
			});

			// Execute the request
			Request->ProcessRequest();
		}
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
			TArray<FString> Sensitive = SensitiveStrings;
			Sensitive.AddUnique(UThirdwebRuntimeSettings::GetEngineAccessToken());
			TArray<uint8> Content = Request->GetContent();
			TW_LOG(
				VeryVerbose,
				TEXT("ThirdwebUtils::Internal::LogRequest::URL=%s | Headers=%s | Content=%s"),
				*Request->GetURL(),
				*UKismetStringLibrary::JoinStringArray(ThirdwebUtils::Internal::MaskSensitiveString(Request->GetAllHeaders(), SensitiveStrings), TEXT(";")),
				ANSI_TO_TCHAR(reinterpret_cast<const char*>(Request->GetContent().GetData()))
			)
		}

		int64 ParseInt64(const FString& String)
		{
			int64 Result;
			return FDefaultValueHelper::ParseInt64(String, Result) ? Result : 0;
		}

		TArray<uint8> StringToBytes(const FString& String)
		{
			uint64 UTF8Length = FPlatformString::ConvertedLength<UTF8CHAR>(*String, String.Len());
			TArray<uint8> Buffer;
			Buffer.SetNumUninitialized(UTF8Length);
			FPlatformString::Convert(reinterpret_cast<UTF8CHAR*>(Buffer.GetData()), Buffer.Num(), *String, String.Len());
			return Buffer;
		}

		UTexture2DDynamic* BytesToTexture2DDynamic(const TArray<uint8>& Bytes)
		{
			IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

			// ReSharper disable CppTooWideScopeInitStatement
			EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(Bytes.GetData(), Bytes.Num());
			if (ImageFormat != EImageFormat::Invalid)
			{
				TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
				if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(Bytes.GetData(), Bytes.Num()))
				{
					TArray64<uint8> RawData;
					if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
					{
						FTexture2DDynamicCreateInfo CreateInfo(PF_B8G8R8A8, false, true);
						UTexture2DDynamic* Texture = UTexture2DDynamic::Create(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), CreateInfo);

						// Filling the texture's data
						Texture->UpdateResource();
						ENQUEUE_RENDER_COMMAND(UpdateDynamicTexture)([Texture, RawDataData = MoveTemp(RawData)](FRHICommandListImmediate&)
						{
							if (FTexture2DDynamicResource* Texture2DDynamicResource = static_cast<FTexture2DDynamicResource*>(Texture->GetResource()))
							{
								uint32 DestStride;
								void* TextureData = RHILockTexture2D(Texture2DDynamicResource->GetTexture2DRHI(), 0, RLM_WriteOnly, DestStride, false);
								FMemory::Memcpy(TextureData, RawDataData.GetData(), RawDataData.Num());
								RHIUnlockTexture2D(Texture2DDynamicResource->GetTexture2DRHI(), 0, false);
							}
						});
						return Texture;
					}
				}
			}
			// ReSharper restore CppTooWideScopeInitStatement
			return nullptr;
		}

		FString BytesToString(const TArray<uint8>& Bytes)
		{
			FUTF8ToTCHAR Data(reinterpret_cast<const ANSICHAR*>(Bytes.GetData()), Bytes.Num());
			return FString(Data.Length(), Data.Get());
		}

		FString GetPluginVersion()
		{
			if (const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("Thirdweb")); Plugin.IsValid())
			{
				return Plugin->GetDescriptor().VersionName;
			}
			return "0.0.0";
		}

		// https://stackoverflow.com/a/58467162/12204515
		FString GenerateUUID()
		{
			static std::random_device Device;
			static std::mt19937 RNG(Device());

			std::uniform_int_distribution Distribution(0, 15);

			// ReSharper disable once CppTooWideScope
			const char* Options = "0123456789abcdef";
			constexpr bool Dash[] = {false, false, false, false, true, false, true, false, true, false, true, false, false, false, false, false};

			std::string Result;
			for (int i = 0; i < 16; i++)
			{
				if (Dash[i]) Result += "-";
				Result += Options[Distribution(RNG)];
				Result += Options[Distribution(RNG)];
			}
			return Result.c_str();
		}

		// ReSharper disable CppPassValueParameterByConstReference
		void SendConnectEvent(const FWalletHandle Wallet)
		{
			if (!IsInGameThread())
			{
				const FWalletHandle WalletCopy = Wallet;
				FFunctionGraphTask::CreateAndDispatchWhenReady([WalletCopy]()
				{
					SendConnectEvent(WalletCopy);
				}, TStatId(), nullptr, ENamedThreads::GameThread);
				return;
			}
			const UThirdwebRuntimeSettings* Settings = UThirdwebRuntimeSettings::Get();
			if (!UThirdwebRuntimeSettings::AnalyticsEnabled() || UThirdwebRuntimeSettings::GetBundleId().IsEmpty() || UThirdwebRuntimeSettings::GetClientId().IsEmpty())
			{
				return;
			}
			FHttpModule& HttpModule = FHttpModule::Get();
			const TSharedRef<IHttpRequest> Request = HttpModule.CreateRequest();
			Request->SetVerb("POST");
			Request->SetURL("https://c.thirdweb.com/event");
			FThirdwebHeaders Headers;
			Headers.SetMany({
				{TEXT("Content-Type"), TEXT("application/json")},
				{TEXT("x-sdk-name"), TEXT("UnrealEngineSDK")},
				{TEXT("x-sdk-os"), UGameplayStatics::GetPlatformName()},
				{TEXT("x-sdk-platform"), TEXT("unreal-engine")},
				{TEXT("x-sdk-version"), GetPluginVersion()},
				{TEXT("x-client-id"), Settings->GetClientId()},
				{TEXT("x-bundle-id"), Settings->GetBundleId()},
			});
			Headers.UpdateRequest(Request);
			Request->SetTimeout(5.0f);

			// ReSharper disable once CppLocalVariableMayBeConst
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
			JsonObject->SetStringField(TEXT("source"), TEXT("connectWallet"));
			JsonObject->SetStringField(TEXT("action"), TEXT("connect"));
			JsonObject->SetStringField(TEXT("walletAddress"), Wallet.ToAddress());
			JsonObject->SetStringField(TEXT("walletType"), Wallet.GetTypeString());
			Request->SetContentAsString(Json::ToString(JsonObject));
			Request->ProcessRequest();
		}

		void SendConnectEvent(const FInAppWalletHandle Wallet) { SendConnectEvent(FWalletHandle(Wallet)); }
		void SendConnectEvent(const FSmartWalletHandle Wallet) { SendConnectEvent(FWalletHandle(Wallet)); }
		// ReSharper restore CppPassValueParameterByConstReference

		TSharedRef<IHttpRequest> CreateEngineRequest(const FString& Verb)
		{
			FHttpModule& HttpModule = FHttpModule::Get();
			const TSharedRef<IHttpRequest> Request = HttpModule.CreateRequest();
			Request->SetVerb(Verb);
			if (!Verb.ToUpper().Equals("GET"))
			{
				Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
			}
			Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
			Request->SetHeader(TEXT("authorization"), TEXT("Bearer ") + UThirdwebRuntimeSettings::GetEngineAccessToken());
			Request->SetTimeout(30.0f);
			return Request;
		}

		FString ReplaceIpfs(const FString& Url, const FString& Gateway)
		{
			static const FString DefaultGateway = TEXT("https://ipfs.io/ipfs/");
			const FString GatewayUrl = Gateway.IsEmpty() ? DefaultGateway : Gateway;
			return Gateway + Url.RightChop(7);
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

		const TMap<EThirdwebEngineTransactionStatus, FText> TransactionStatusToText = {
			{EThirdwebEngineTransactionStatus::Queued, LOCTEXT("Queued", "Queued")},
			{EThirdwebEngineTransactionStatus::Sent, LOCTEXT("Sent", "Sent")},
			{EThirdwebEngineTransactionStatus::Mined, LOCTEXT("Mined", "Mined")},
			{EThirdwebEngineTransactionStatus::Errored, LOCTEXT("Errored", "Errored")},
			{EThirdwebEngineTransactionStatus::Cancelled, LOCTEXT("Cancelled", "Cancelled")}
		};

		const TMap<EThirdwebEngineTransactionOnChainStatus, FText> OnChainStatusToText = {
			{EThirdwebEngineTransactionOnChainStatus::Success, LOCTEXT("Success", "Success")},
			{EThirdwebEngineTransactionOnChainStatus::Reverted, LOCTEXT("Reverted", "Reverted")}
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
			return MakeShareable(new FJsonObject);
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

		FString ToString(const TArray<TSharedPtr<FJsonValue>>& JsonValueArray)
		{
			FString Out;
			const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Out);
			FJsonSerializer::Serialize(JsonValueArray, Writer);
			return Out;
		}

		FString ToString(const TSharedPtr<FJsonValue>& JsonValue)
		{
			if (JsonValue.IsValid())
			{
				switch (JsonValue->Type)
				{
				case EJson::String: return JsonValue->AsString();
				case EJson::Boolean: return JsonValue->AsBool() ? TEXT("true") : TEXT("false");
				case EJson::Array: return ToString(JsonValue->AsArray());
				case EJson::Object: return ToString(JsonValue->AsObject());
				case EJson::Number: return FString::SanitizeFloat(JsonValue->AsNumber(), 0);
				default: return TEXT("");
				}
			}
			return TEXT("");
		}

		FString AsString(const TSharedPtr<FJsonValue>& JsonValue)
		{
			if (JsonValue.IsValid() && !JsonValue->IsNull())
			{
				switch (JsonValue->Type)
				{
				case EJson::String: return JsonValue->AsString();
				case EJson::Number: return FString::Printf(TEXT("%f"), JsonValue->AsNumber());
				case EJson::Boolean: return JsonValue->AsBool() ? TEXT("true") : TEXT("false");
				case EJson::Object: return ToString(JsonValue->AsObject());
				case EJson::Array: return ToString(JsonValue->AsArray());
				default: return TEXT("");
				}
			}
			return TEXT("");
		}

		bool ParseEngineResponse(const FString& Content, TSharedPtr<FJsonValue>& JsonValue, FString& Error)
		{
			if (TSharedPtr<FJsonObject> ContentJsonObject = ToJson(Content); ContentJsonObject.IsValid())
			{
				if (ContentJsonObject->HasTypedField<EJson::Object>(TEXT("error")))
				{
					Error = ParseEngineError(ContentJsonObject->GetObjectField(TEXT("error")));
					return false;
				}
				if (ContentJsonObject->HasField(TEXT("result")))
				{
					JsonValue = ContentJsonObject->TryGetField(TEXT("result"));
					if (JsonValue.IsValid())
					{
						return true;
					}
					Error = TEXT("Invalid Result Type");
					return false;
				}
			}
			Error = TEXT("Invalid Response");
			return false;
		}

		bool ParseEngineResponse(const FString& Content, TSharedPtr<FJsonObject>& JsonObject, FString& Error)
		{
			if (TSharedPtr<FJsonValue> JsonValue; ParseEngineResponse(Content, JsonValue, Error))
			{
				JsonObject = JsonValue->AsObject();
				return true;
			}
			return false;
		}

		bool ParseEngineResponse(const FString& Content, TArray<TSharedPtr<FJsonValue>>& JsonArray, FString& Error)
		{
			if (TSharedPtr<FJsonValue> JsonValue; ParseEngineResponse(Content, JsonValue, Error))
			{
				JsonArray = JsonValue->AsArray();
				return true;
			}
			return false;
		}

		FString ParseEngineError(const TSharedPtr<FJsonObject>& Error)
		{
			FString Message = TEXT("Unknown Error");
			if (Error->HasTypedField<EJson::String>(TEXT("message")))
			{
				Message = Error->GetStringField(TEXT("message"));
			}
			return Message;
		}
	}
}

#undef LOCTEXT_NAMESPACE
