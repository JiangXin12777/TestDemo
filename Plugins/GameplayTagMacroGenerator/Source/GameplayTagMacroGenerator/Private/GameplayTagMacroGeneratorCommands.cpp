// Copyright (C) 2023 Shpz<shaaaaapz@gmail.com>. All Rights Reserved.

#include "GameplayTagMacroGeneratorCommands.h"

#define LOCTEXT_NAMESPACE "FGameplayTagMacroGeneratorModule"

void FGameplayTagMacroGeneratorCommands::RegisterCommands()
{
#if ENGINE_MAJOR_VERSION >= 5
	UI_COMMAND(PluginAction, "Generate GameplayTag Macro", "Generate GameplayTag Macro", EUserInterfaceActionType::Button, FInputChord());
#else
	UI_COMMAND(PluginAction, "Generate GameplayTag Macro", "Generate GameplayTag Macro", EUserInterfaceActionType::Button, FInputChord());
#endif
}

#undef LOCTEXT_NAMESPACE
