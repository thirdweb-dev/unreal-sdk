// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebWalletHandle.h"
#include "Engine/StreamableManager.h"
#include "ThirdwebInAppWalletHandle.generated.h"

struct FThirdwebLinkedAccount;
enum class EThirdwebOTPMethod : uint8;
enum class EThirdwebOAuthProvider : uint8;

USTRUCT(BlueprintType, Blueprintable)
struct THIRDWEB_API FInAppWalletHandle : public FWalletHandle
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FCreateInAppWalletDelegate, const FInAppWalletHandle&);
	DECLARE_DELEGATE_OneParam(FGetLinkedAccountsDelegate, const TArray<FThirdwebLinkedAccount>&);

	enum EInAppSource
	{
		InvalidSource,
		OAuthProvider,
		Email,
		Phone,
		Jwt,
		AuthEndpoint,
		Siwe,
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
	 * Creates an email-based in-app wallet.
	 *
	 * @param Email The email address to associate with the wallet, must be a valid email format.
	 * @param SuccessDelegate Delegate to execute upon successful wallet creation.
	 * @param ErrorDelegate Delegate to execute upon encountering an error during wallet creation.
	 */
	static void CreateEmailWallet(const FString& Email, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Initiates the creation of an OAuth wallet using the specified provider.
	 *
	 * @param Provider The OAuth provider to use for wallet creation.
	 * @param SuccessDelegate A delegate to be called upon successful wallet creation.
	 * @param ErrorDelegate A delegate to be called if an error occurs during wallet creation.
	 */
	static void CreateOAuthWallet(const EThirdwebOAuthProvider Provider, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Creates a phone-based in-app wallet.
	 *
	 * @param Phone The phone number used to create the wallet, which must be a valid phone number.
	 * @param SuccessDelegate The delegate called upon successful wallet creation.
	 * @param ErrorDelegate The delegate called if there is an error during wallet creation.
	 */
	static void CreatePhoneWallet(const FString& Phone, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Creates a JWT wallet.
	 *
	 * @param SuccessDelegate The delegate to be executed upon successful wallet creation.
	 * @param ErrorDelegate The delegate to be executed if an error occurs during wallet creation.
	 */
	static void CreateJwtWallet(const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate) { return CreateCustomAuthWallet(Jwt, SuccessDelegate, ErrorDelegate); }

	/**
	 * Creates a Sign-In with Ethereum (SIWE) wallet.
	 *
	 * @param SuccessDelegate The delegate to be executed upon successful wallet creation.
	 * @param ErrorDelegate The delegate to be executed if an error occurs during wallet creation.
	 */
	static void CreateSiweWallet(const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Creates an in-app wallet with an authentication endpoint.
	 *
	 * @param SuccessDelegate Delegate to be called upon successful wallet creation, receiving specific success details.
	 * @param ErrorDelegate Delegate to be called upon an error during wallet creation, receiving the error message.
	 */
	static void CreateAuthEndpointWallet(const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
	{
		return CreateCustomAuthWallet(AuthEndpoint, SuccessDelegate, ErrorDelegate);
	}

	/**
	 * Creates a guest wallet by delegating to the custom authorization wallet creation function.
	 *
	 * @param SuccessDelegate Delegate to be called upon successful wallet creation.
	 * @param ErrorDelegate Delegate to be called if there is an error during the wallet creation process.
	 */
	static void CreateGuestWallet(const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate) { return CreateCustomAuthWallet(Guest, SuccessDelegate, ErrorDelegate); }

private:
	/**
	 * Creates a custom authenticated in-app wallet.
	 *
	 * @param Source The source from where the in-app wallet is to be created.
	 * @param SuccessDelegate Delegate to be called upon successful creation of the wallet.
	 * @param ErrorDelegate Delegate to be called if an error occurs during the wallet creation.
	 */
	static void CreateCustomAuthWallet(const EInAppSource Source, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

public:
	/** Check if the wallet is connected to a session */
	bool IsConnected() const;

	/** Disconnect a wallet from a session */
	void Disconnect() const;

	/**
	 * Sends a one-time password (OTP) to the wallet handle's associated phone or email.
	 *
	 * @param SuccessDelegate Delegate to be executed on successful OTP sending.
	 * @param ErrorDelegate Delegate to be executed if an error occurs during OTP sending.
	 *
	 */
	void SendOTP(const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Signs in with a One-Time Password (OTP) for the in-app wallet handle.
	 *
	 * @param OTP The one-time password to be verified.
	 * @param SuccessDelegate The delegate to execute upon successful verification.
	 * @param ErrorDelegate The delegate to execute if verification fails or an error occurs.
	 */
	void SignInWithOTP(const FString& OTP, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Links a One-Time Password (OTP) to the specified in-app wallet.
	 *
	 * @param Wallet The in-app wallet handle to which the OTP will be linked.
	 * @param OTP The one-time password that will be linked to the wallet.
	 * @param SuccessDelegate The delegate to be called upon successful linking of the OTP.
	 * @param ErrorDelegate The delegate to be called if there is an error linking the OTP.
	 */
	void LinkOTP(const FInAppWalletHandle& Wallet, const FString& OTP, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

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
	 * Sign in with OAuth for the in-app wallet handle.
	 *
	 * @param AuthResult The result string obtained from OAuth authentication.
	 * @param SuccessDelegate Delegate to be executed upon successful sign-in.
	 * @param ErrorDelegate Delegate to be executed upon failure, with an error message.
	 */
	void SignInWithOAuth(const FString& AuthResult, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Links a new OAuth wallet to the existing in-app wallet.
	 *
	 * @param Wallet The in-app wallet handle that the OAuth account will link with. Must be a valid handle. Must never have independently authenticated.
	 * @param AuthResult The authentication result string containing the OAuth credentials.
	 * @param SuccessDelegate A delegate that will be called upon successfully linking the OAuth account.
	 * @param ErrorDelegate A delegate that will be called if there is an error during the linking process, providing an error message.
	 */
	void LinkOAuth(const FInAppWalletHandle& Wallet, const FString& AuthResult, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Signs in with a JSON Web Token (JWT) for authentication.
	 *
	 * @param Jwt The JSON Web Token used for authentication; must be a valid JWT string.
	 * @param SuccessDelegate Delegate to be executed upon successful authentication, must be bound.
	 * @param ErrorDelegate Delegate to be executed if an error occurs during authentication.
	 */
	void SignInWithJwt(const FString& Jwt, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
 	 * Links a JSON Web Token (JWT) for authentication.
 	 *
	 * @param Wallet The in-app wallet handle that the JWT account will link with. Must be a valid handle. Must never have independently authenticated.
 	 * @param Jwt The JSON Web Token used for authentication; must be a valid JWT string.
 	 * @param SuccessDelegate Delegate to be executed upon successful authentication, must be bound.
 	 * @param ErrorDelegate Delegate to be executed if an error occurs during authentication.
 	 */
	void LinkJwt(const FInAppWalletHandle& Wallet, const FString& Jwt, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Signs in with the authentication endpoint using the provided payload.
	 *
	 * @param Payload The authentication payload, must be a valid JSON string.
	 * @param SuccessDelegate The delegate to be executed on successful sign-in, must be bound.
	 * @param ErrorDelegate The delegate to be executed if an error occurs, should be bound to handle error messages.
	 */
	void SignInWithAuthEndpoint(const FString& Payload, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * links an authentication endpoint using the provided payload.
	 *
	 * @param Wallet The in-app wallet handle that the AuthEndpoint account will link with. Must be a valid handle. Must never have independently authenticated.
	 * @param Payload The authentication payload, must be a valid JSON string.
	 * @param SuccessDelegate The delegate to be executed on successful sign-in, must be bound.
	 * @param ErrorDelegate The delegate to be executed if an error occurs, should be bound to handle error messages.
	 */
	void LinkAuthEndpoint(const FInAppWalletHandle& Wallet, const FString& Payload, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Signs in with guest authentication.
	 *
	 * @param SuccessDelegate The delegate to execute on successful sign-in.
	 * @param ErrorDelegate The delegate to execute if an error occurs during the sign-in process; will receive an error message.
	 */
	void SignInWithGuest(const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Links guest authentication.
	 *
	 * @param Wallet The in-app wallet handle that the guest account will link with. Must be a valid handle. Must never have independently authenticated.
	 * @param SuccessDelegate The delegate to execute on successful sign-in.
	 * @param ErrorDelegate The delegate to execute if an error occurs during the sign-in process; will receive an error message.
	 */
	void LinkGuest(const FInAppWalletHandle& Wallet, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Initiates a sign-in process using Ethereum credentials.
	 *
	 * @param Payload The payload containing the sign-in data.
	 * @param Signature The signature authenticating the payload.
	 * @param SuccessDelegate Delegate to execute upon successful sign-in.
	 * @param ErrorDelegate Delegate to execute upon encountering an error.
	 */
	void SignInWithEthereum(const FString& Payload, const FString& Signature, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Links an in-app wallet using the SIWE (Sign-In with Ethereum) protocol.
	 *
	 * @param Wallet The in-app wallet handle to be linked, must be a valid wallet.
	 * @param Payload The payload containing the sign-in data.
	 * @param Signature The signature authenticating the payload.
	 * @param SuccessDelegate The delegate to be executed upon successful linking.
	 * @param ErrorDelegate The delegate to be executed in case of an error during the linking process.
	 */
	void LinkSiwe(const FInAppWalletHandle& Wallet, const FString& Payload, const FString& Signature, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);
	
	/**
	 * Retrieves the linked accounts associated with this FInAppWalletHandle.
	 *
	 * @param SuccessDelegate Delegate that will be executed with the linked accounts information if the operation is successful.
	 * @param ErrorDelegate Delegate that will be executed with an error message if the operation fails.
	 */
	void GetLinkedAccounts(const FGetLinkedAccountsDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/**
	 * Retrieves the source of the in-app wallet.
	 *
	 * @return The source of the in-app wallet as an EInAppSource enum.
	 */
	EInAppSource GetSource() const { return Source; }

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
			{Siwe, TEXT("SIWE")},
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
};
