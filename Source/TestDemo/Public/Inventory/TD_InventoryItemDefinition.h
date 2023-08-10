#pragma once

#include "CoreMinimal.h"
#include "TD_InventoryItemDefinition.generated.h"

UCLASS(Blueprintable, Const, Abstract)
class UTD_InventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	UTD_InventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 通过 UClass 查找当前实例 */
	const UTD_InventoryItemFragment* FindFragmentByClass(TSubclassOf<UTD_InventoryItemFragment> FragmentClass) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UTD_InventoryItemFragment>> Fragments;
};

UCLASS()
class UTD_InventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UTD_InventoryItemFragment* FindItemDefinitionFragment(TSubclassOf<UTD_InventoryItemDefinition> ItemDef, TSubclassOf<UTD_InventoryItemFragment> FragmentClass);
};