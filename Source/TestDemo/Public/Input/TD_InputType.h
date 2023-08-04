#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "TD_InputType.generated.h"

class UTD_InputHandle;
class UTD_InputManagerComponent;
class UTD_InputSetupDataAsset;

/**
 * 存储稍后可以访问的已注册绑定。
 */
USTRUCT(BlueprintType)
struct FTD_ProcessedBinding
{
	GENERATED_BODY()

	/** 此绑定正在观察的输入操作。 */
	UPROPERTY()
	TObjectPtr<const UInputAction> InputAction;

	/** 触发此绑定正在观察的事件。 */
	ETriggerEvent TriggerEvent;

	/** 此绑定的句柄，稍后可用于将其删除。 */
	FEnhancedInputActionEventBinding* Handle;

	FTD_ProcessedBinding()
	{
		InputAction = nullptr;
		TriggerEvent = ETriggerEvent::None;
		Handle = nullptr;
	}
	
	explicit FTD_ProcessedBinding(const UInputAction* InputAction, const ETriggerEvent TriggerEvent, FEnhancedInputActionEventBinding* Handle)
		: InputAction(InputAction)
		, TriggerEvent(TriggerEvent)
		, Handle(Handle)
	{
	}
};

/**
 * 有关完全处理的输入设置的信息。
 */
USTRUCT(BlueprintType)
struct FTD_ProcessedInputSetup
{
	GENERATED_BODY()

	/** Data Asset that has been processed. */
	UPROPERTY()
	TObjectPtr<const UTD_InputSetupDataAsset> SourceData;

	/** All bindings created for this setup. */
	UPROPERTY()
	TArray<FTD_ProcessedBinding> ProcessedBindings;
    
	FTD_ProcessedInputSetup()
	{
		SourceData = nullptr;
		ProcessedBindings.Reset();
	}
	
	explicit FTD_ProcessedInputSetup(const UTD_InputSetupDataAsset* SourceData, const TArray<FTD_ProcessedBinding>& ProcessedBindings)
		: SourceData(SourceData)
		, ProcessedBindings(ProcessedBindings)
	{
	}
};

/**
 * 表示可以稍后执行的缓冲命令。
 */
USTRUCT(BlueprintType)
struct FTD_BufferedInputCommand
{
    GENERATED_BODY()

    /** 注册此命令的管理员。 */
    UPROPERTY(BlueprintReadOnly, Category = "Input Command")
    TObjectPtr<UTD_InputManagerComponent> Source;
    
    /** 将聚合所有命令的输入操作。 */
    UPROPERTY(BlueprintReadOnly, Category = "Input Command")
    TObjectPtr<const UInputAction> InputAction;

    /** 必须执行的处理程序。 */
    UPROPERTY(BlueprintReadOnly, Category = "Input Command")
    TObjectPtr<const UTD_InputHandle> Handler;

    /** 需要处理的值。 */
    UPROPERTY(BlueprintReadOnly, Category = "Input Command")
    FInputActionValue Value;

    /** 发起操作的触发器。 */
    UPROPERTY(BlueprintReadOnly, Category = "Input Command")
    ETriggerEvent TriggerEvent;

    FTD_BufferedInputCommand()
    {
    	Source = nullptr;
        InputAction = nullptr;
        Handler = nullptr;
        Value.Reset();
        TriggerEvent = ETriggerEvent::None;
    }

    explicit FTD_BufferedInputCommand(UTD_InputManagerComponent* Source
        , const UInputAction* Action
        , const UTD_InputHandle* Handler
        , const FInputActionValue& Value
        , const ETriggerEvent TriggerEvent)
        : Source(Source)
        , InputAction(Action)
        , Handler(Handler)
        , Value(Value)
        , TriggerEvent(TriggerEvent)
    {
    }

    /** 评估该命令是否具有有效数据并且可以执行。 */
    bool IsValid() const;
    
    /** 在给定的上下文中执行此条目表示的命令。 */
    void Execute() const;

    FORCEINLINE bool operator == (const FTD_BufferedInputCommand& In) const
    {
        return In.InputAction == InputAction && In.Handler == Handler && In.TriggerEvent == TriggerEvent;
    }
    FORCEINLINE bool operator != (const FTD_BufferedInputCommand& In) const
    {
        return In.InputAction != InputAction || In.Handler != Handler || In.TriggerEvent != TriggerEvent;
    }
};