// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/AsyncTaskThirdwebLoginWithOAuth.h"

#include "ThirdwebLog.h"
#include "ThirdwebOAuthBrowser.h"
#include "ThirdwebSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Misc/DateTime.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void UAsyncTaskThirdwebLoginWithOAuth::Activate()
{
	Browser->OnSuccess.AddDynamic(this, &ThisClass::HandleSuccess);
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
	Task->Browser =  CreateWidget<UThirdwebOAuthBrowser>(UGameplayStatics::GetGameInstance(WorldContextObject), UThirdwebOAuthBrowser::StaticClass());
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebLoginWithOAuth::HandleFailed(FString Error)
{
	Failed.Broadcast(Error);
	Browser->RemoveFromParent();
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebLoginWithOAuth::HandleSuccess()
{
	Success.Broadcast(TEXT(""));
	Browser->RemoveFromParent();
	SetReadyToDestroy();
}
