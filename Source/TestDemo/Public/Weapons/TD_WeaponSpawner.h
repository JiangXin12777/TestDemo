#pragma once

#include "CoreMinimal.h"
#include "TD_WeaponSpawner.generated.h"

class UTD_WeaponPickupDefinition;
class UTD_InventoryItemDefinition;
class UStaticMeshComponent;
class UCapsuleComponent;

/**
 * Weapon generator.
 */
UCLASS()
class ATD_WeaponSpawner : public AActor
{
	GENERATED_BODY()
public:
	ATD_WeaponSpawner(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	/** 当武器生成时，检查范围内是否存在玩家。 */
	void CheckForExistingOverlaps();

	/**
	 * 尝试拿起武器
	 * @param Pawn 接收道具的玩家
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "TD|WeaponPickup")
	void AttemptPickUpWeapon(APawn* Pawn);

	/**
	 * （装载武器）
	 * @param WeaponItemClass 定义要生成的拾取 Actor、授予的能力以及要添加的标签
	 * @param ReceivingPawn 接收道具的玩家
	 *
	 * @return 返回玩家是否接收道具成功
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "TD|WeaponPickup", meta = (DisplayName = "GiveWeapon"))
	bool k2_GiveWeapon(TSubclassOf<UTD_InventoryItemDefinition> WeaponItemClass, APawn* ReceivingPawn);

	/** 开启生成计时 */
	void StartCoolDown();

	/** 重置生成计时 */
	UFUNCTION(BlueprintCallable, Category = "TD|WeaponPickup")
	void ResetCoolDown();

	/** 冷却时间完成后调用 */
	UFUNCTION()
	void OnCoolDownTimerComplete();

	/** 设置武器模型的是否可见 */
	void SetWeaponPickupVisibility(bool bShouldBeVisible);

	/** 播放拾取时的音效和特效 */
	UFUNCTION(BlueprintNativeEvent, Category = "TD|WeaponPickup")
	void PlayPickupEffects();

	/** 播放武器重生时的音效和特效 */
	UFUNCTION(BlueprintNativeEvent, Category = "TD|WeaponPickup")
	void PlayRespawnEffects();

	/** 当武器的可用状态变化时，网络复制主动调用。 */
	UFUNCTION()
	void OnRep_WeaponAvailability();

	/** 在项目定义类型中搜索匹配的统计信息并返回值，如果未找到则返回 0 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TD|WeaponPickup")
	static int32 GetDefaultStatFromItemDef(const TSubclassOf<UTD_InventoryItemDefinition> WeaponItemClass, FGameplayTag StatTag);

public:
	/** 触发生成的碰撞框 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|WeaponPickup")
	TObjectPtr<UCapsuleComponent> CollisionVolume;

	/** 展示底座 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|WeaponPickup")
	TObjectPtr<UStaticMeshComponent> PadMesh;

	/** 武器模型 */
	UPROPERTY(BlueprintReadOnly, Category = "TD|WeaponPickup")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	/** 展示时的旋转速度 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TD|WeaponPickup")
	float WeaponMeshRotationSpeed;

	/** 冷却时间句柄（生成武器） */
	FTimerHandle CoolDownTimerHandle;

	FTimerHandle CheckOverlapsDelayTimerHandle;
	
protected:
	// ~Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	// ~End AActor Interface

	//~UObject interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of UObject interface

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

protected:
	/** 武器资产配置数据 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "TD|WeaponPickup")
	TObjectPtr<UTD_WeaponPickupDefinition> WeaponDefinition;

	/** 当前武器是否可以使用 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_WeaponAvailability, Category = "TD|WeaponPickup")
	bool bIsWeaponAvailable;

	/** 拾取后再次生成武器的冷却时间 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|WeaponPickup")
	float CoolDownTime;

	/** 武器可用之间的延迟和当我们检查生成器中是否有Pawn.  用于给 bIsWeaponAvailable OnRep 时间发射和播放 FX。 */ 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|WeaponPickup")
	float CheckExistingOverlapDelay;

	/** 用于驱动武器重生时间指示器0-1 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TD|WeaponPickup")
	float CoolDownPercentage;
};