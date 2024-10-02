// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

#include "K2Node/K2Node_VerifyOTP.h"

#include "ThirdwebFunctionLibrary.h"

UK2Node_VerifyOTP::UK2Node_VerifyOTP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FunctionReference.SetExternalMember(GET_FUNCTION_NAME_CHECKED(UThirdwebFunctionLibrary, BP_VerifyOTP), UThirdwebFunctionLibrary::StaticClass());
}
