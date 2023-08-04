#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputTriggers.h"
#include "Engine/DeveloperSettings.h"
#include "TD_InputSetting.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Input Config"))
class UTD_InputSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UTD_InputSetting(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;

public:
    /** 由管理器跟踪并委托给注册处理程序的输入事件。 */
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Core Settings")
    TArray<ETriggerEvent> TrackedEvents;
    
	/** 游戏标签（如果存在）会阻止摄像机移动。 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input Blocking")
	FGameplayTag BlockCameraTag;

	/**
	 * 游戏标签（如果存在）会阻止角色移动。
	 * 此标签还会阻止与移动相关的操作（例如跳跃）以及角色移动模式的更改（即从站立到蹲伏，反之亦然）。
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input Blocking")
	FGameplayTag BlockMovementTag;

	/** 游戏标签（如果存在）会阻止角色旋转。 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input Blocking")
	FGameplayTag BlockRotationTag;

	/** 代表游戏手柄输入模式的游戏标签。 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input Modes", DisplayName = "Input Mode: Gamepad")
	FGameplayTag GamepadInputModeTag;

	/** 代表键盘和鼠标输入模式的游戏标签。 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input Modes", DisplayName = "Input Mode: Keyboard and Mouse")
	FGameplayTag KeyboardAndMouseInputModeTag;

    /**
     * 启用设置资产的数据验证。
     * 强烈建议保持启用状态以避免设置数据中的常见错误！
     */
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Development")
    bool bEnableDataValidation;

    /**
     * 如果启用，它将确保上下文中的所有输入操作都有一个等效的处理程序。
     * 仅当启用数据验证时才能使用。
     */
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Development", meta = (EditCondition = bEnableDataValidation, EditConditionHides))
    bool bMatchHandlersWithContext;
    
    /**
     * 确定是否将调试消息显示添加到所有输入处理程序的屏幕上。
     * 该选项仅在编辑器中有效，不会添加到打包版本中。
     */
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Development")
    bool bShowScreenDebugMessages;

    /** 屏幕上调试消息的持续时间。 */
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Development", meta = (EditCondition = bShowScreenDebugMessages, EditConditionHides))
    float DebugMessageDuration;
    
    /** 用于屏幕调试消息的颜色。 */
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Development", meta = (EditCondition = bShowScreenDebugMessages, EditConditionHides))
    FColor DebugMessageColor;
};