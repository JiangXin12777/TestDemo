#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInputComponent.h"
#include "TD_InputSetupDataAsset.generated.h"

class UInputMappingContext;
class UTD_InputHandle;

UCLASS(ClassGroup = "TD|Input")
class UTD_InputSetupDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UTD_InputSetupDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	    
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	/** 检查此数据资产是否具有与给定 ActionTrigger 兼容的输入处理程序。
	 *  注：不同执行逻辑的 IH 不可以使用同一个 IA。
	 */
	bool HasCompatibleInputHandler(const UInputAction* InputAction, const ETriggerEvent& TriggerEvent) const;

public:
	/** 为此输入映射上下文确定的优先级。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Input Setup")
	int32 Priority;
    
	/** 此配置添加的输入映射上下文。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Input Setup")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	/** 与此输入映射上下文相关的输入处理程序的所有实例。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Input Setup")
	TArray<TObjectPtr<UTD_InputHandle>> InputHandlers;
};