// Copyright (C) 2023 Shpz<shaaaaapz@gmail.com>. All Rights Reserved.

#include "GameplayTagMacroGeneratorStyle.h"
#include "GameplayTagMacroGenerator.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

#if ENGINE_MAJOR_VERSION >= 5
#include "Styling/SlateStyleMacros.h"
#define RootToContentDir Style->RootToContentDir
#endif

TSharedPtr<FSlateStyleSet> FGameplayTagMacroGeneratorStyle::StyleInstance = nullptr;

void FGameplayTagMacroGeneratorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FGameplayTagMacroGeneratorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FGameplayTagMacroGeneratorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("GameplayTagMacroGeneratorStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedRef<FSlateStyleSet> FGameplayTagMacroGeneratorStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("GameplayTagMacroGeneratorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("GameplayTagMacroGenerator")->GetBaseDir() / TEXT("Resources"));

#if ENGINE_MAJOR_VERSION >= 5
	Style->Set("GameplayTagMacroGenerator.PluginAction", new IMAGE_BRUSH(TEXT("Icon20"), Icon20x20));
#else
	Style->Set(
		"GameplayTagMacroGenerator.PluginAction",
		new FSlateImageBrush(
			Style->RootToContentDir(TEXT("Icon40"), TEXT(".png")),
			Icon40x40));
#endif
	return Style;
}

void FGameplayTagMacroGeneratorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle &FGameplayTagMacroGeneratorStyle::Get()
{
	return *StyleInstance;
}
