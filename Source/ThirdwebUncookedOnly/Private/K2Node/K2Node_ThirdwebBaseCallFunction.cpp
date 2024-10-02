// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

#include "K2Node/K2Node_ThirdwebBaseCallFunction.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"

FLinearColor UK2Node_ThirdwebBaseCallFunction::GetNodeTitleColor() const
{
	return FLinearColor(FColor::FromHex(TEXT("#7207b9")));
}

FSlateIcon UK2Node_ThirdwebBaseCallFunction::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = FLinearColor(FColor::FromHex(TEXT("#f213a4")));
	static const FSlateIcon Icon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");;
	return Icon;
}

void UK2Node_ThirdwebBaseCallFunction::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions(ActionRegistrar);
	if (UClass* Action = GetClass(); ActionRegistrar.IsOpenForRegistration(Action))
	{
		UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(GetClass());
		check(Spawner != nullptr);
		ActionRegistrar.AddBlueprintAction(Action, Spawner);
	}
}
