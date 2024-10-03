// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_ThirdwebBaseCallFunction.h"
#include "K2Node_VerifyOTP.generated.h"

/**
 * 
 */
UCLASS()
class THIRDWEBUNCOOKEDONLY_API UK2Node_VerifyOTP : public UK2Node_ThirdwebBaseCallFunction
{
	GENERATED_BODY()

public:
	UK2Node_VerifyOTP(const FObjectInitializer& ObjectInitializer);

	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
};
