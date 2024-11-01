// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Browser/Android/ThirdwebAndroidJNI.h"

#if PLATFORM_ANDROID
#include "ThirdwebLog.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Browser/ThirdwebOAuthBrowserUserWidget.h"
#include "Android/AndroidPlatform.h"
#include "Engine/GameEngine.h"

void ThirdwebUtils::Internal::Android::CallJniStaticVoidMethod(JNIEnv* Env, const jclass Class, jmethodID Method, ...)
{
	TW_LOG(VeryVerbose, TEXT("ThirdwebUtils::Internal::Android::CallJniStaticVoidMethod::Called"))
	va_list Args;
	va_start(Args, Method);
	Env->CallStaticVoidMethodV(Class, Method, Args);
	va_end(Args);
	Env->DeleteLocalRef(Class);
}

UThirdwebOAuthBrowserUserWidget* GetOAuthBrowserUserWidget()
{
	if (UGameEngine* GameEngine = Cast<UGameEngine>(GEngine))
	{
		if (TObjectPtr<UGameViewportClient> Viewport = GameEngine->GameViewport; IsValid(Viewport))
		{
			if (UWorld* World = Viewport->GetWorld())
			{
				TArray<UUserWidget*> FoundWidgets;
				UWidgetBlueprintLibrary::GetAllWidgetsOfClass(
					World,
					FoundWidgets,
					UThirdwebOAuthBrowserUserWidget::StaticClass(),
					false
				);
				TW_LOG(
					VeryVerbose,
					TEXT("ThirdwebAndroidJNI::GetOAuthBrowserUserWidget::Found %d UThirdwebOAuthBrowserUserWidgets"),
					FoundWidgets.Num()
				)
				TArray<UThirdwebOAuthBrowserUserWidget*> Browsers;
				for (int i = 0; i < FoundWidgets.Num(); i++)
				{
					if (UThirdwebOAuthBrowserUserWidget* Browser = Cast<UThirdwebOAuthBrowserUserWidget>(
						FoundWidgets[i]))
					{
						Browsers.Emplace(Browser);
					}
				}
				UThirdwebOAuthBrowserUserWidget* ActiveBrowser = nullptr;
				if (Browsers.Num() > 0)
				{
					ActiveBrowser = Browsers[0];
					if (Browsers.Num() > 1)
					{
						TW_LOG(
							Warning,
							TEXT(
								"ThirdwebAndroidJNI::GetOAuthBrowserUserWidget::Expected to find 1 Browser but found %d"
							),
							FoundWidgets.Num()
						);
					}
				}
				return ActiveBrowser;
			}
			TW_LOG(Error, TEXT("ThirdwebAndroidJNI::GetOAuthBrowserUserWidget::Game Viewport Invalid"));
		}
		else TW_LOG(Error, TEXT("ThirdwebAndroidJNI::GetOAuthBrowserUserWidget::Game World Invalid"));
	}
	else TW_LOG(Error, TEXT("ThirdwebAndroidJNI::GetOAuthBrowserUserWidget::Game Engine Invalid"));

	return nullptr;
}

JNI_METHOD void Java_com_epicgames_unreal_GameActivity_handleDeepLink(JNIEnv* Env, jobject, jstring JDeepLink)
{
	if (!Env->IsSameObject(JDeepLink, NULL))
	{
		const char* CDeepLink = Env->GetStringUTFChars(JDeepLink, NULL);
		const FString DeepLink = FString(UTF8_TO_TCHAR(CDeepLink));

		if (UThirdwebOAuthBrowserUserWidget* Browser = GetOAuthBrowserUserWidget())
		{
			Browser->HandleDeepLink(DeepLink);
		}

		Env->ReleaseStringUTFChars(JDeepLink, CDeepLink);
	}
}

JNI_METHOD void Java_com_epicgames_unreal_GameActivity_handleOnCustomTabsDismissed(JNIEnv* Env, jobject, jstring JUrl)
{
	if (!Env->IsSameObject(JUrl, NULL))
	{
		if (UThirdwebOAuthBrowserUserWidget* Browser = GetOAuthBrowserUserWidget())
		{
			const char* CUrl = Env->GetStringUTFChars(JUrl, NULL);
			const FString Url = FString(UTF8_TO_TCHAR(CUrl));
			Browser->HandleCustomTabsDismissed(Url);
		}
	}
}
#endif
