// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Containers/UnrealString.h"

class FTWECheckAction
{
	FString ActionId;
	FString ActionDisplayName;

public:
	FTWECheckAction(const FString& InActionId, const FString& InActionDisplayName): ActionId(InActionId), ActionDisplayName(InActionDisplayName)
	{
	}

	const FString& GetActionId() const { return ActionId; }
	const FString& GetActionDisplayName() const { return ActionDisplayName; }
};

class FTWECheckEntry
{
	FString CheckId;
	FString CheckMessage;
	TArray<FTWECheckAction> CheckActions;

public:
	FTWECheckEntry(const FString& InCheckId, const FString& InCheckMessage, const TArray<FTWECheckAction>& InCheckActions)
		: CheckId(InCheckId), CheckMessage(InCheckMessage), CheckActions(InCheckActions)
	{
	}

	const FString& GetCheckId() const { return CheckId; }
	const FString& GetCheckMessage() const { return CheckMessage; }
	const TArray<FTWECheckAction>& GetCheckActions() const { return CheckActions; }
};

class ITWECheck
{
protected:
	static const TArray<FTWECheckEntry> EmptyEntries;

public:
	ITWECheck() = default;
	UE_NONCOPYABLE(ITWECheck);

	virtual ~ITWECheck()
	{
	}

	virtual bool ShouldTick() const { return false; }
	virtual TArray<FTWECheckEntry> Tick(float DeltaSeconds) const{return EmptyEntries;}
	
	virtual TArray<FTWECheckEntry> ProcessCustomSignal(const FString& Context, const FString& SignalId) const {return EmptyEntries;}
	virtual TArray<FTWECheckEntry> ProcessLogMessage(FString InLogLevel, const FString& Category, const FString& Message) const { return EmptyEntries; }
	virtual void HandleAction(const FString& CheckId, const FString& ActionId) const
	{
	}
};
