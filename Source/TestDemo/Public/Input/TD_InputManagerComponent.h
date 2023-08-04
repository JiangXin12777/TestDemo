#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Input/TD_InputType.h"
#include "TD_InputManagerComponent.generated.h"

class UTD_InputSetupDataAsset;
class UAbilitySystemComponent;
class UArrowComponent;

UCLASS(ClassGroup = "TD|Input", meta = (BlueprintSpawnableComponent))
class UTD_InputManagerComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	/**
	* 用于查找代表前进方向的箭头组件的标签名称。
	* 这对于自上而下或类似的游戏很有用，在这些游戏中控制器或演员方向向量无法正确反映应移动的方向。
	*/
	static FName ForwardReferenceTag;

public:
	UTD_InputManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 获取当前所有者所装载的技能组件 */
	UFUNCTION(BlueprintPure, Category = "TD|Input Component")
	FORCEINLINE UAbilitySystemComponent* GetOwnerAbilitySystemComponent() const { return OwnerAbilitySystemComponent.Get(); }

	/** 注册当前所有者所装载的技能组件 */
	UFUNCTION(BlueprintCallable, Category = "TD|Input Component")
	void RegisterOwnerAbilitySystemComponent(UAbilitySystemComponent* InVal);

protected:
	//~ Begin UActorComponent Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End UActorComponent Interface
	
	/** 提供所有者的前向参考。 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "TD|Input Component")
	FVector GetForwardVector() const;

	/** 提供所有者的正确参考。 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "TD|Input Component")
	FVector GetRightVector() const;

    /**
     * 提供所有者处理的最后一个输入向量。
     * 默认情况下，它将从所有者的 Pawn 移动组件中检索值，但这可以通过在与此组件相关的 Pawn 中实现“最后输入向量提供者接口”来覆盖。
     */
    UFUNCTION(BlueprintPure, Category = "TD|Input Component")
    FVector GetLastInputVector() const;
    
	/** 提供拥有该组件的 pawn。 */
	UFUNCTION(BlueprintPure, Category = "TD|Input Component")
	APawn* GetPawn() const;

	/** 提供拥有该组件的控制器。 */
	UFUNCTION(BlueprintPure, Category = "TD|Input Component")
	AController* GetController() const;

    /** 检查该组件是否正在与本地控制器一起运行。 */
    UFUNCTION(BlueprintPure, Category = "TD|Input Component")
    bool IsLocallyControlled() const;

    /** 检索提供的控制器的增强型输入子系统。 */
    UFUNCTION(BlueprintPure, Category = "TD|Input Component")
    UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem(AController* Controller) const;
    
    /** 检查给定的设置数据是否已注册到该组件。 */
    UFUNCTION(BlueprintPure, Category = "TD|Input Component")
    bool HasSetupData(const UTD_InputSetupDataAsset* SetupData) const;
    
	/** 检查给定的输入映射上下文是否已为当前所有者注册。 */
	UFUNCTION(BlueprintPure, Category = "TD|Input Component")
	bool HasInputMappingContext(const UInputMappingContext* InputMappingContext) const;

    /** 检查此组件是否具有与给定 ActionTrigger 事件兼容的处理程序。 */
    UFUNCTION(BlueprintPure, Category = "TD|Input Component")
    bool HasCompatibleInputHandler(const UInputAction* InputAction, const ETriggerEvent& TriggerEvent) const;

    /** 处理设置数据，注册其输入上下文和处理程序。 */
    UFUNCTION(BlueprintCallable, Category = "TD|Input Component")
    void AddInputSetupData(const UTD_InputSetupDataAsset* SetupData);

    /** 删除先前注册的设置数据。 */
    UFUNCTION(BlueprintCallable, Category = "TD|Input Component")
    void RemoveInputSetupData(const UTD_InputSetupDataAsset* SetupData);
    
    /** 向所有者的 ASC 发送游戏事件。 */
    UFUNCTION(BlueprintCallable, Category = "TD|Input Component")
    int32 SendGameplayEventToOwner(const FGameplayTag& GameplayEventTag, const FInputActionValue& Value,
        const UInputAction* InputAction, bool bSendLocally = true, bool bSendToServer = true) const;
    
protected:

	/** 绑定到该管理器的输入组件。 */
	UPROPERTY(Transient)
	TObjectPtr<UEnhancedInputComponent> InputComponent;

    /** 将有助于此输入管理器设置的所有数据资产。 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TD|Input Component")
    TArray<TObjectPtr<UTD_InputSetupDataAsset>> InputHandlerSetup;

protected:
    /** 组件初始化的入口点。 */
    void SetupInputComponent(const APawn* Pawn);
    
    /** 注册新的输入映射上下文并处理必要的绑定。 */
    void AddInputMappingContext(const UInputMappingContext* InputMappingContext, int32 Priority, TArray<FTD_ProcessedBinding>& OutBindings);

    /** Dispatches an action to a registered Input Handler for the Started Event. */
    UFUNCTION()
    void DispatchStartedEvent(const FInputActionInstance& ActionInstance);

    /** Dispatches an action to a registered Input Handler for the Started Event. */
    UFUNCTION()
    void DispatchTriggeredEvent(const FInputActionInstance& ActionInstance);

    /** Dispatches an action to a registered Input Handler for the Ongoing Event. */
    UFUNCTION()
    void DispatchOngoingEvent(const FInputActionInstance& ActionInstance);

    /** Dispatches an action to a registered Input Handler for the Ongoing Event. */
    UFUNCTION()
    void DispatchCompletedEvent(const FInputActionInstance& ActionInstance);

    /** Dispatches an action to a registered Input Handler for the Ongoing Event. */
    UFUNCTION()
    void DispatchCancelledEvent(const FInputActionInstance& ActionInstance);
    
    /** Effectively dispatches the event, using the Actual Trigger. */
    void Dispatch(const FInputActionInstance& ActionInstance, ETriggerEvent ActualTrigger);
    
    /** 删除输入映射上下文及其绑定。 */
    void RemoveInputMappingContext(const UInputMappingContext* InputMappingContext);

    /** 清除分配给该组件的整个输入设置。 */
    virtual void ClearInputSetup();
    
	/** 当拥有的 Pawn 重新启动时调用，允许该组件重新创建绑定。 */
	UFUNCTION()
	void OnPawnRestarted(APawn* Pawn);

	/**
	 * Invoked when the pawn's controller changes, allowing this component to clear current bindings.
	 */
	UFUNCTION()
	void OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);	

    /**
     * Provides a vector reference for a given axis.
     *
     * This is necessary for all handlers that requires movement direction such as "Movement".
     */
    UFUNCTION(BlueprintNativeEvent, Category = "TD|Input Component")
    void GetVectorForAxis(const EAxis::Type Axis, FVector& OutReference) const;

private:
	/** 控制器当前分配给我们的所有者。 */
	UPROPERTY()
	TObjectPtr<AController> OwnerController;

	/** 当前所有者的前向引用。 */
	UPROPERTY()
	TObjectPtr<UArrowComponent> ForwardReference;

    /** 所有注册到该组件的设置，都通过其映射上下文进行映射。 */
    UPROPERTY()
    TMap<TObjectPtr<UInputMappingContext>, FTD_ProcessedInputSetup> ProcessedSetups;

	/** 当前所有者所装载的技能组件 */
	UPROPERTY(Transient)
	TWeakObjectPtr<UAbilitySystemComponent> OwnerAbilitySystemComponent;

private:
    /**
     * 当我们是本地自治代理时，允许将游戏事件发送到服务器。
     */
    UFUNCTION(Server, Reliable)
    void Server_SendGameplayEventToOwner(const FGameplayTag& GameplayEventTag,
        const FInputActionValue& Value, const UInputAction* InputAction) const;
    
    /**
     * Allows sending a gameplay event to client when we are a remote server (!).
     * This is a very unlikely scenario, just added for the sake of being thorough.
     */
    UFUNCTION(Client, Reliable)
    void Client_SendGameplayEventToOwner(const FGameplayTag& GameplayEventTag,
        const FInputActionValue& Value, const UInputAction* InputAction) const;
};