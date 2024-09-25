// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebWalletHandle.h"
#include "ThirdwebInAppWalletHandle.generated.h"

enum class EThirdwebOTPMethod : uint8;
enum class EThirdwebOAuthProvider : uint8;

USTRUCT(BlueprintType, Blueprintable)
struct THIRDWEB_API FInAppWalletHandle : public FWalletHandle
{
	GENERATED_BODY()

	enum EInAppSource
	{
		InvalidSource,
		OAuthProvider,
		Email,
		Phone,
		Jwt,
		AuthEndpoint,
		Guest
	};

	FInAppWalletHandle();

private:
	explicit FInAppWalletHandle(const EInAppSource InSource, const int64 InID);
	explicit FInAppWalletHandle(const EInAppSource InSource, const FString& Int64String);
	/** For OAuthProvider Initialization */
	explicit FInAppWalletHandle(const EThirdwebOAuthProvider InProvider, const FString& Int64String);

public:
	virtual bool IsValid() const override { return Super::IsValid() && Source != InvalidSource; }

	/** Explicitly clear handle */
	virtual void Invalidate() override
	{
		Super::Invalidate();
		Source = InvalidSource;
	}

	/**
	 * Creates an in-app email wallet.
	 *
	 * @param Email The email address to associate with the wallet.
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreateEmailWallet(const FString& Email, FInAppWalletHandle& Wallet, FString& Error);

	/**
	 * Creates an in-app OAuth wallet using the specified provider.
	 *
	 * @param Provider The OAuth provider to use for wallet creation.
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreateOAuthWallet(const EThirdwebOAuthProvider Provider, FInAppWalletHandle& Wallet, FString& Error);

	/**
	 * Creates an in-app phone wallet.
	 *
	 * @param Phone The phone number to associate with the wallet.
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreatePhoneWallet(const FString& Phone, FInAppWalletHandle& Wallet, FString& Error);

	static bool CreateJwtWallet(FInAppWalletHandle& Wallet, FString& Error) { return CreateCustomAuthWallet(Jwt, Wallet, Error); }
	static bool CreateAuthEndpointWallet(FInAppWalletHandle& Wallet, FString& Error) { return CreateCustomAuthWallet(AuthEndpoint, Wallet, Error); }
	static bool CreateGuestWallet(FInAppWalletHandle& Wallet, FString& Error) { return CreateCustomAuthWallet(Guest, Wallet, Error); }

private:
	static bool CreateCustomAuthWallet(const EInAppSource Source, FInAppWalletHandle& Wallet, FString& Error);
	
public:

	/** Check if the wallet is connected to a session */
	bool IsConnected() const;

	/** Disconnect a wallet from a session */
	void Disconnect() const;

	bool VerifyOTP(const EThirdwebOTPMethod Method, const FString& OTP, FString& Error);
	bool SendOTP(const EThirdwebOTPMethod Method, FString& Error);

	bool FetchOAuthLoginURL(const FString& RedirectUrl, FString& LoginLink, FString& Error);
	bool SignInWithOAuth(const FString& AuthResult, FString& Error);
	/**
	 * Signs in using a JSON Web Token (JWT) and an encryption key.
	 *
	 * @param Jwt The JSON Web Token post-authenticating the user. This is any OIDC compatible JWT.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the sign-in is successful, false otherwise.
	 */
	bool SignInWithJwt(const FString& Jwt, FString& Error);

	/**
	 * Signs in using any payload.
	 *
	 * @param Payload The data payload required for the sign-in process. The payload can have any shape, but is generally JSON.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the sign-in is successful, false otherwise.
	 */
	bool SignInWithAuthEndpoint(const FString& Payload, FString& Error);

	/**
	 * Signs in as an anonymous guest user using a semi-reproducible device fingerprint.
	 * Fingerprints can change from update to update and should not be relied upon for long term use.
	 *
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the sign-in is successful, false otherwise.
	 */
	bool SignInWithGuest(FString& Error);

	const TCHAR* GetSourceString() const { return GetSourceString(Source); }
	
	static const TCHAR* GetSourceString(const EInAppSource Source)
	{
		const TMap<EInAppSource, const TCHAR*> Map = {
			{InvalidSource, TEXT("Invalid")},
			{OAuthProvider, TEXT("OAuthProvider")},
			{Email, TEXT("Email")},
			{Phone, TEXT("Phone")},
			{Jwt, TEXT("JWT")},
			{AuthEndpoint, TEXT("AuthEndpoint")},
			{Guest, TEXT("Guest")},
		};
		return Map.Contains(Source) ? Map[Source] : TEXT("Unknown");
	}

	virtual FString GetDisplayName() const override;

	EThirdwebOAuthProvider GetOAuthProvider() const { return Provider; }

private:
	// The current Handle type
	EInAppSource Source = InvalidSource;
	EThirdwebOAuthProvider Provider;
};
