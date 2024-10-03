// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "K2Node/K2Node_VerifyOTP.h"

#include "EdGraphToken.h"
#include "KismetCompiler.h"
#include "ThirdwebFunctionLibrary.h"

UK2Node_VerifyOTP::UK2Node_VerifyOTP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FunctionReference.SetExternalMember(GET_FUNCTION_NAME_CHECKED(UThirdwebFunctionLibrary, BP_VerifyOTP), UThirdwebFunctionLibrary::StaticClass());
}

void UK2Node_VerifyOTP::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
	CompilerContext.MessageLog.Error(*NSLOCTEXT("Thirdweb", "ObjectToCheck_NotConnected", "Object To Check is not connected. @@").ToString(), this);
	if (UEdGraphPin* Pin = FindPin(FName(TEXT("Wallet"))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Pin!"))
		//If not, generate a compiler error.
		//If you want it to just be a warning instead, replace Error with Warning.
		CompilerContext.MessageLog.Error(*NSLOCTEXT("Thirdweb", "ObjectToCheck_NotConnected", "Object To Check is not connected. @@").ToString(), this);
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Did not Find pin"))
	}
}
