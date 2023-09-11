#pragma once

#include "CoreMinimal.h"
#include "ComboExamine.h"
#include "GameplayTagContainer.h"
#include "TD_FightComponent.generated.h"

class UAbilitySystemComponent;

/**
 * 战斗组件
 */
UCLASS()
class UTD_FightComponent
	: public UActorComponent
{
	GENERATED_BODY()
	
public:
	UTD_FightComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 获取当前的 ASC */
	UFUNCTION(BlueprintCallable, Category = "TD|Abilities")
	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	template <class T>
	FORCEINLINE T* GetAbilitySystemComponent() const
	{
		return CastChecked<T>(GetAbilitySystemComponent());
	}

	UFUNCTION(BlueprintCallable, Category = "TD|Fight")
	void TryComboAttack(TSubclassOf<UGameplayAbility> InAbilityClass);

	UFUNCTION(BlueprintCallable, Category = "TD|Fight")
	void DoAttack(TSubclassOf<UGameplayAbility> InAbilityClass) const;

	UFUNCTION(BlueprintCallable, Category = "TD|Fight")
	void Released();

	UFUNCTION(BlueprintCallable, Category = "TD|Fight")
	void TryBlock();
	
	UFUNCTION(BlueprintCallable, Category = "TD|Fight")
	void TryRoll();

	const FComboExamine& GetComboExamine() const { return ComboExamine; }
	FComboExamine& GetComboExamine() { return ComboExamine; }

protected:
	UFUNCTION(BlueprintCallable, Category = "TD|Abilities")
	bool IsActiveAbilitiesWithClass(TSubclassOf<UGameplayAbility> InAbilityClass) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TD|Combo")
	FComboExamine ComboExamine;
};