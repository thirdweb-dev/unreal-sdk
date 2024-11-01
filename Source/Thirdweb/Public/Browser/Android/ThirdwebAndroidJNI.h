// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"

extern "C"
{
	JNI_METHOD void Java_com_epicgames_unreal_GameActivity_handleDeepLink(JNIEnv*, jobject, jstring);
	JNI_METHOD void Java_com_epicgames_unreal_GameActivity_handleOnCustomTabsDismissed(JNIEnv*, jobject, jstring);
}

namespace ThirdwebUtils::Internal::Android
{
	extern void CallJniStaticVoidMethod(JNIEnv *Env, const jclass Class, jmethodID Method, ...);
}
#endif