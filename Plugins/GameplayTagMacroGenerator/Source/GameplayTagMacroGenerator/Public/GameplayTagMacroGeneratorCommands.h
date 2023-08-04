// Copyright (C) 2023 Shpz<shaaaaapz@gmail.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "GameplayTagMacroGeneratorStyle.h"

class FGameplayTagMacroGeneratorCommands : public TCommands<FGameplayTagMacroGeneratorCommands>
{
public:
	FGameplayTagMacroGeneratorCommands()
		: TCommands<FGameplayTagMacroGeneratorCommands>(TEXT("GameplayTagMacroGenerator"), NSLOCTEXT("Contexts", "GameplayTagMacroGenerator", "GameplayTag Macro Generator"), NAME_None, FGameplayTagMacroGeneratorStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> PluginAction;
};
