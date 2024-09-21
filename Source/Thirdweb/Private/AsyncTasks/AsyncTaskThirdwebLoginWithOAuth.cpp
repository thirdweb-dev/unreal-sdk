// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/AsyncTaskThirdwebLoginWithOAuth.h"

#include "ThirdwebLog.h"
#include "TimerManager.h"

#include "Blueprint/UserWidget.h"

#include "Browser/ThirdwebOAuthBrowserUserWidget.h"

#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"

#include "Misc/DateTime.h"

void UAsyncTaskThirdwebLoginWithOAuth::Activate()
{
	Browser->OnAuthenticated.AddDynamic(this, &ThisClass::HandleAuthenticated);
	Browser->OnError.AddDynamic(this, &ThisClass::HandleFailed);
	Browser->AddToViewport(10000);
	Browser->Authenticate(Wallet);
}

UAsyncTaskThirdwebLoginWithOAuth* UAsyncTaskThirdwebLoginWithOAuth::LoginWithOAuth(UObject* WorldContextObject, const FWalletHandle& Wallet)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	UAsyncTaskThirdwebLoginWithOAuth* Task = NewObject<UAsyncTaskThirdwebLoginWithOAuth>(WorldContextObject);
	Task->Wallet = Wallet;
	Task->Browser = CreateWidget<UThirdwebOAuthBrowserUserWidget>(UGameplayStatics::GetGameInstance(WorldContextObject), UThirdwebOAuthBrowserUserWidget::StaticClass());
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebLoginWithOAuth::HandleFailed(const FString& Error)
{
	Failed.Broadcast(Error);
	Browser->RemoveFromParent();
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebLoginWithOAuth::HandleAuthenticated(const FString& AuthResult)
{
	if (FString Error; !Wallet.SignInWithOAuth(AuthResult, Error))
	{
		return HandleFailed(Error);
	}
	Success.Broadcast(TEXT(""));
	Browser->RemoveFromParent();
	SetReadyToDestroy();
}

