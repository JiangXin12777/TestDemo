#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TD_InputHandle.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = "TD|Input")
class UTD_InputHandle : public UObject
{
	GENERATED_BODY()

public:
	UTD_InputHandle(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    // -- Begin Object implementation
    virtual UWorld* GetWorld() const override;
    // -- End Object implementation
    
	/** 检查此处理程序是否可以处理给定的操作触发器。 */
	UFUNCTION(BlueprintNativeEvent, Category = "TD|Input Handler")
	bool CanHandle(const ETriggerEvent& TriggerEvent, const UInputAction* InputAction) const;
    
	/**
	 * 处理由增强输入子系统触发的输入操作。
	 * 不必重写此方法来处理触发器。相反，请考虑为每个相关的触发事件实现每个专用处理程序方法。
	 *
	 * @param Manager 分配给调用此处理程序的所属角色的 Actor 组件。
	 * @param Value 应用于拥有角色的最终动作值。
	 * @param TriggerEvent 触发此处理程序的事件类型。
	 * @param InputAction 有关触发此处理程序的输入操作的其他信息。
	 */
	void HandleInput(UTD_InputManagerComponent* Manager, const FInputActionValue& Value, const ETriggerEvent& TriggerEvent, const UInputAction* InputAction) const;

    /** 通知此处理程序是否可以缓冲。通知此输入处理程序是否可以缓冲，这意味着如果输入缓冲区打开，则不会立即执行。相反，它将稍后在缓冲区关闭并释放时执行。 */
    UFUNCTION(BlueprintPure, Category = "TD|Input Handler")
    bool CanBeBuffered() const;

    /** 设置世界指针以便于访问。旨在由输入管理器调用。 */
    void SetWorld(UWorld* WorldReference);

protected:
	/** 可能触发此输入的操作。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Input Handler")
	TArray<TObjectPtr<UInputAction>> InputActions;

	/** 将调用处理程序的触发器。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Input Handler")
	TArray<ETriggerEvent> TriggerEvents;

    /** 通知此处理程序是否可以缓冲。 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Input Handler")
    bool bCanBeBuffered;

	/** Handles the Triggered event. */
	UFUNCTION(BlueprintNativeEvent, Category = "TD|Input Handler")
	void HandleTriggeredEvent(UTD_InputManagerComponent* Manager, const FInputActionValue& Value, const UInputAction* InputAction) const;

	/** Handles the Started event. */	
	UFUNCTION(BlueprintNativeEvent, Category = "TD|Input Handler")
	void HandleStartedEvent(UTD_InputManagerComponent* Manager, const FInputActionValue& Value, const UInputAction* InputAction) const;

	/** Handles the Ongoing event. */	
	UFUNCTION(BlueprintNativeEvent, Category = "TD|Input Handler")
	void HandleOngoingEvent(UTD_InputManagerComponent* Manager, const FInputActionValue& Value, const UInputAction* InputAction) const;

	/** Handles the Cancelled event. */	
	UFUNCTION(BlueprintNativeEvent, Category = "TD|Input Handler")
	void HandleCancelledEvent(UTD_InputManagerComponent* Manager, const FInputActionValue& Value, const UInputAction* InputAction) const;

	/** Handles the Completed event. */	
	UFUNCTION(BlueprintNativeEvent, Category = "TD|Input Handler")
	void HandleCompletedEvent(UTD_InputManagerComponent* Manager, const FInputActionValue& Value, const UInputAction* InputAction) const;

	/** 检查所有者的 ASC 中是否存在任何提供的标签。 */
	UFUNCTION(BlueprintPure, Category = "TD|Input Handler")
	static bool HasAnyTags(const UTD_InputManagerComponent* Manager, const FGameplayTagContainer& Tags);

private:
    /** 对世界指针的弱引用。在管理器触发的所有执行期间应该有效。 */
    TWeakObjectPtr<UWorld> WorldPtr;
    
    void AddOnScreenDebugMessage(const UTD_InputManagerComponent* Manager, const FInputActionValue& Value,
        const ETriggerEvent& TriggerEvent, const UInputAction* InputAction) const;
};