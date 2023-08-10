#pragma once

#include "CoreMinimal.h"
#include "TD_WeaponActor.generated.h"

class UArrowComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class UDidItHitActorComponent;

/**
 * 武器 Actor
 */
UCLASS(Blueprintable)
class ATD_WeaponActor : public AActor
{
	GENERATED_BODY()

public:
	ATD_WeaponActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "TD|Weapon")
	void ToggleTraceCheck(bool bTrace);

	UFUNCTION(BlueprintCallable, Category = "TD|Weapon")
	void SetupVariables(UPrimitiveComponent* MyPrimitiveTarget, AActor* MyActorTarget);

	UFUNCTION(BlueprintPure, Category = "TD|Weapon")
	UDidItHitActorComponent* GetDidItHitActorComponent() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TD|Weapon")
	void OnAddHitItem(FHitResult NewHitResult);
	
	//注意，这里是根据物理资产的名称来判断的，所以如果你的物理资产名称不是这些，那么就会返回EWeaponHitBodyResult::UpperBody
	//UFUNCTION(BlueprintPure, BlueprintCallable, Category = "TD|Weapon")
	//EWeaponHitBodyResult GetHitBodyResultByBoneName(FName InBoneName);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Weapon")
	TObjectPtr<UDidItHitActorComponent> DidItHitActorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Weapon")
	TObjectPtr<UArrowComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponStaticMesh;
};