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
	/**
	 * Constructs an FInAppWalletHandle object.
	 *
	 * @param InSource The source of the in-app wallet, must be a valid source.
	 * @param InID The unique identifier for the wallet handle, must be greater than 0.
	 * @return An initialized FInAppWalletHandle instance.
	 */
	explicit FInAppWalletHandle(const EInAppSource InSource, const int64 InID);
	
	/**
	 * Constructs an FInAppWalletHandle object.
	 *
	 * @param InSource The source of the in-app wallet. Must not be InvalidSource or OAuthProvider.
	 * @param Int64String A string representing the unique identifier for the wallet handle. Must be parsable as a positive integer.
	 * @return An initialized FInAppWalletHandle instance.
	 */
	explicit FInAppWalletHandle(const EInAppSource InSource, const FString& Int64String);

	/**
	 * Constructs an FInAppWalletHandle object.
	 *
	 * @param InProvider The OAuth provider used for the in-app wallet, must not be none.
	 * @param Int64String The string representation of the unique identifier for the wallet handle, must represent a value greater than 0.
	 * @return An initialized FInAppWalletHandle instance.
	 */
	explicit FInAppWalletHandle(const EThirdwebOAuthProvider InProvider, const FString& Int64String);

public:
	/**
	 * Checks if the InAppWalletHandle is valid.
	 *
	 * @return True if the InAppWalletHandle is valid; false otherwise.
	 */
	virtual bool IsValid() const override;

	/**
	 * Invalidates the current wallet handle.
	 *
	 * Overrides the base class method to mark the wallet handle as invalid,
	 * setting the source to an invalid state.
	 */
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
	static bool CreateEcosystemEmailWallet(const FString& PartnerId, const FString& Email, FInAppWalletHandle& Wallet, FString& Error);

	/**
	 * Creates an in-app OAuth wallet using the specified provider.
	 *
	 * @param Provider The OAuth provider to use for wallet creation.
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreateOAuthWallet(const EThirdwebOAuthProvider Provider, FInAppWalletHandle& Wallet, FString& Error);
	static bool CreateEcosystemOAuthWallet(const FString& PartnerId, const EThirdwebOAuthProvider Provider, FInAppWalletHandle& Wallet, FString& Error);

	/**
	 * Creates an in-app phone wallet.
	 *
	 * @param Phone The phone number to associate with the wallet.
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreatePhoneWallet(const FString& Phone, FInAppWalletHandle& Wallet, FString& Error);
	static bool CreateEcosystemPhoneWallet(const FString& PartnerId, const FString& Phone, FInAppWalletHandle& Wallet, FString& Error);

	/**
	 * Creates an in-app JWT wallet.
	 *
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreateJwtWallet(FInAppWalletHandle& Wallet, FString& Error) { return CreateCustomAuthWallet(Jwt, Wallet, Error); }
	static bool CreateEcosystemJwtWallet(const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error) { return CreateEcosystemCustomAuthWallet(PartnerId, Jwt, Wallet, Error); }
	/**
	 * Creates an in-app wallet using an authentication endpoint.
	 *
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreateAuthEndpointWallet(FInAppWalletHandle& Wallet, FString& Error) { return CreateCustomAuthWallet(AuthEndpoint, Wallet, Error); }
	static bool CreateEcosystemAuthEndpointWallet(const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error) { return CreateEcosystemCustomAuthWallet(PartnerId, AuthEndpoint, Wallet, Error); }

	/**
	 * Creates an in-app guest wallet.
	 *
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreateGuestWallet(FInAppWalletHandle& Wallet, FString& Error) { return CreateCustomAuthWallet(Guest, Wallet, Error); }
	static bool CreateEcosystemGuestWallet(const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error) { return CreateEcosystemCustomAuthWallet(PartnerId, Guest, Wallet, Error); }

private:
	/**
	 * Creates a custom authenticated in-app wallet.
	 *
	 * @param Source The source of authentication. The valid values are Jwt, AuthEndpoint, and Guest.
	 * @param Wallet Reference to an FInAppWalletHandle object that will be initialized if authentication succeeds.
	 * @param Error Reference to an FString that will contain an error message if authentication fails.
	 * @return True if the wallet is successfully created; otherwise, false.
	 */
	static bool CreateCustomAuthWallet(const EInAppSource Source, FInAppWalletHandle& Wallet, FString& Error);
	static bool CreateEcosystemCustomAuthWallet(const FString& PartnerId, const EInAppSource Source, FInAppWalletHandle& Wallet, FString& Error);
	
public:

	/** Check if the wallet is connected to a session */
	bool IsConnected() const;

	/** Disconnect a wallet from a session */
	void Disconnect() const;

	/**
	 * Verifies a One Time Password (OTP) for the in-app wallet handle.
	 *
	 * @param OTP The One Time Password to verify.
	 * @param Error Output parameter that will contain an error message if the verification fails.
	 * @return True if the OTP is successfully verified, false otherwise.
	 */
	bool VerifyOTP(const FString& OTP, FString& Error);

	/**
	 * Sends a One-Time Password (OTP) for the in-app wallet handle.
	 *
	 * @param Error Returns an error message if the OTP sending fails.
	 * @return True if the OTP was sent successfully, false otherwise.
	 */
	bool SendOTP(FString& Error);

	/**
	 * Fetches the OAuth login URL for the in-app wallet.
	 *
	 * @param RedirectUrl The URL to which the OAuth provider should redirect after authentication.
	 * @param LoginLink The resulting login link from the OAuth provider.
	 * @param Error If the operation fails, the error message will be stored here.
	 * @return True if the login URL was successfully fetched, otherwise false.
	 */
	bool FetchOAuthLoginURL(const FString& RedirectUrl, FString& LoginLink, FString& Error);

	/**
	 * Signs in using OAuth authentication details.
	 *
	 * @param AuthResult The authentication result string obtained from the OAuth provider.
	 * @param Error Will contain an error message if the sign-in process fails.
	 * @return True if sign-in was successful, false otherwise.
	 */
	bool SignInWithOAuth(const FString& AuthResult, FString& Error);
	
	/**
	 * Signs in using a JSON Web Token (JWT).
	 *
	 * @param Jwt The JSON Web Token post-authenticating the user. This is any OIDC compatible JWT. The token should be a string starting with ey.
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

	/**
	 * Retrieves the source string.
	 *
	 * @return The source string.
	 */
	const TCHAR* GetSourceString() const { return GetSourceString(Source); }

	/**
	 * Retrieves a string representation of the specified EInAppSource.
	 *
	 * @param Source The source type to get the string representation for.
	 *
	 * @return A TCHAR pointer to the string representation of the source type.
	 *         If the source type is invalid or unrecognized, returns "Unknown".
	 */
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

	/**
	 * Retrieves the display name of the in-app wallet handle.
	 *
	 * @return A formatted string representing the display name of the in-app wallet handle. If the handle is invalid, returns "InAppWallet::INVALID".
	 */
	virtual FString GetDisplayName() const override;

	/**
	 * Retrieves the configured OAuth provider.
	 *
	 * @return The configured OAuth provider.
	 */
	EThirdwebOAuthProvider GetOAuthProvider() const { return Provider; }

private:
	// The current Handle type
	EInAppSource Source = InvalidSource;
	EThirdwebOAuthProvider Provider;
	FString EcosystemPartnerId;
};
