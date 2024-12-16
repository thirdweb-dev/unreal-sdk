// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ITWECheck.h"

class UThirdwebRuntimeSettings;

class FTWEEssentialSettingsCheck : public ITWECheck
{
public:
	FTWEEssentialSettingsCheck()
	{
	}

	UE_NONCOPYABLE(FTWEEssentialSettingsCheck);

	virtual ~FTWEEssentialSettingsCheck()
	{
	}

	virtual bool ShouldTick() const override { return true; }
	virtual TArray<FTWECheckEntry> Tick(float DeltaSeconds) const override;
	virtual void HandleAction(const FString& CheckId, const FString& ActionId) const override;
};
