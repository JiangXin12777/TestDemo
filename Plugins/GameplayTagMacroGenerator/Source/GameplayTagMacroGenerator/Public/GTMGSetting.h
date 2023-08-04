// Copyright (C) 2023 Shpz<shaaaaapz@gmail.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GTMGSetting.generated.h"

/**
 *	注释样式
 *
 *	Comment style
 */
UENUM(BlueprintType)
enum ECommentStyle
{
	/**
	 *	文档注释
	 *	
	 *	Documentation comments
	 */
	Documentation,

	/**
	 *	行注释
	 *	
	 *	Line comments
	 */
	Line
};

/**
 *	GameplayTagMacroGenerator 设置
 *
 *	GameplayTagMacroGenerator settings
 */
UCLASS(config = GameplayTagMacroGeneratorSetting, DefaultConfig)
class GAMEPLAYTAGMACROGENERATOR_API UGTMGSetting : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(
		config,
		EditAnywhere,
		Category = "GameplayTag Macro Generator Setting",
		meta = (ToolTip = "C++ 宏文件输出路径。\nC++ macro file output path."))
	FDirectoryPath CppMacroFileOutputPath;

	UPROPERTY(
		config,
		EditAnywhere,
		Category = "GameplayTag Macro Generator Setting",
		meta = (ToolTip = "C++ 宏文件输出名。\nC++ macro file output name."))
	FString CppMacroFileOutputName = "MyGameplayTags";

	UPROPERTY(
		config,
		EditAnywhere,
		Category = "GameplayTag Macro Generator Setting",
		meta = (ToolTip = "C++ 宏注释样式。\nC++ macro comment style."))
	TEnumAsByte<ECommentStyle> CommentStyle = ECommentStyle::Documentation;
};
