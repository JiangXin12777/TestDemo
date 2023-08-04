// Copyright (C) 2023 Shpz<shaaaaapz@gmail.com>. All Rights Reserved.

#include "GameplayTagMacroGenerator.h"
#include "GameplayTagMacroGeneratorStyle.h"
#include "GameplayTagMacroGeneratorCommands.h"
#include "GameplayTagsModule.h"
#include "GameplayTagsSettings.h"
#include "GTMGCpp.h"
#include "GTMGInterface.h"
#include "GTMGSetting.h"
#include "ISettingsModule.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName GameplayTagMacroGeneratorTabName("GameplayTagMacroGenerator");

#define LOCTEXT_NAMESPACE "FGameplayTagMacroGeneratorModule"

void FGameplayTagMacroGeneratorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FGameplayTagMacroGeneratorStyle::Initialize();
	FGameplayTagMacroGeneratorStyle::ReloadTextures();

	FGameplayTagMacroGeneratorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGameplayTagMacroGeneratorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FGameplayTagMacroGeneratorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGameplayTagMacroGeneratorModule::RegisterMenus));

	RegisterSetting();
}

void FGameplayTagMacroGeneratorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGameplayTagMacroGeneratorStyle::Shutdown();

	FGameplayTagMacroGeneratorCommands::Unregister();
}

void FGameplayTagMacroGeneratorModule::PluginButtonClicked()
{
	if (CheckSourceValid() && CheckOutputValid())
	{
		IGTMGInterface* Generator = nullptr;

		// 这里判定代码生成器宏是否定义
		// judge whether the code generator macro is defined
#ifdef GTMG_CPP
		Generator = new FGTMGCpp();
#endif

		if (Generator)
		{
			// fix: GameplayTagCompositeDataTable - GameplayTagDataTable 新增子项，GameplayTagList 没有刷新
			// fix: When GameplayTagCompositeDataTable - GameplayTagDataTable add new item，GameplayTagList not refreshed
			IGameplayTagsModule::OnTagSettingsChanged.Broadcast();

			// 调用生成函数
			// Call Generate function
			Generator->Generate();
		}
	}
}

void FGameplayTagMacroGeneratorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FGameplayTagMacroGeneratorCommands::Get().PluginAction, PluginCommands);
		}
	}

#if ENGINE_MAJOR_VERSION >= 5
	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(
					FToolMenuEntry::InitToolBarButton(FGameplayTagMacroGeneratorCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
#else
	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FGameplayTagMacroGeneratorCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
#endif
}

void FGameplayTagMacroGeneratorModule::RegisterSetting() const
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(TEXT("Settings"));
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings(
			"Project",
			"Project",
			"GameplayTag Macro Generator",
			LOCTEXT("GameplayTagMacroGenerator", "GameplayTag Macro Generator"),
			LOCTEXT("GameplayTagMacroGeneratorSettingDescription", "GameplayTag Macro Generator."),
			GetMutableDefault<UGTMGSetting>()
		);
	}
}

bool FGameplayTagMacroGeneratorModule::CheckSourceValid() const
{
	const UGameplayTagsSettings* Settings = GetDefault<UGameplayTagsSettings>();
	if (Settings->GameplayTagTableList.Num() < 1 && Settings->GameplayTagList.Num() < 1)
	{
		FText DialogText = LOCTEXT("GameplayTagIsEmpty",
		                           "GameplayTagTableList and GameplayTagList are all empty. \nPlease check Project Settings - Project - GameplayTags.");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return false;
	}
	return true;
}

bool FGameplayTagMacroGeneratorModule::CheckOutputValid() const
{
	FDirectoryPath OutputPath = GetDefault<UGTMGSetting>()->CppMacroFileOutputPath;
	if (OutputPath.Path.IsEmpty())
	{
		FText DialogText = LOCTEXT("CppMacroFileOutputPathIsEmpty",
		                           "C++ macro file output path cannot be empty. \nPlease check Project Settings - Gameplay Tag Macro Generator - Cpp Macro File Output Path.");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return false;
	}

	if (FPaths::DirectoryExists(OutputPath.Path))
	{
		return true;
	}
	else
	{
		FText DialogText = LOCTEXT("CppMacroFileOutputPathNotExists",
		                           "C++ macro file output path not exists. \nPlease check Project Settings - Gameplay Tag Macro Generator - Cpp Macro File Output Path.");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return false;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameplayTagMacroGeneratorModule, GameplayTagMacroGenerator)
