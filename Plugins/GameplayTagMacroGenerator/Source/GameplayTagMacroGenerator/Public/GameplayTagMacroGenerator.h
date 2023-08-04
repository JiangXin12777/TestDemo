// Copyright (C) 2023 Shpz<shaaaaapz@gmail.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FGameplayTagMacroGeneratorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void PluginButtonClicked();

private:
	void RegisterMenus();
	void RegisterSetting() const ;
	bool CheckSourceValid() const;
	bool CheckOutputValid() const;

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
