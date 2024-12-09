// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebLoginWithOAuth.h"

#include "ThirdwebLog.h"
#include "TimerManager.h"
#include "Async/TaskGraphInterfaces.h"
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

UAsyncTaskThirdwebLoginWithOAuth* UAsyncTaskThirdwebLoginWithOAuth::LoginWithOAuth(UObject* WorldContextObject, const FInAppWalletHandle& Wallet)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->Wallet = Wallet;
	Task->Browser = CreateWidget<UThirdwebOAuthBrowserUserWidget>(UGameplayStatics::GetGameInstance(WorldContextObject), UThirdwebOAuthBrowserUserWidget::StaticClass());
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebLoginWithOAuth::HandleAuthenticated(const FString& AuthResult)
{
	Wallet.SignInWithOAuth(AuthResult, BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleSignedIn), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

void UAsyncTaskThirdwebLoginWithOAuth::HandleSignedIn()
{
	if (IsInGameThread())
	{
		Success.Broadcast(TEXT(""));
		Browser->RemoveFromParent();
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebLoginWithOAuth> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleSignedIn();
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UAsyncTaskThirdwebLoginWithOAuth::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Browser->RemoveFromParent();
		Failed.Broadcast(Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebLoginWithOAuth> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
