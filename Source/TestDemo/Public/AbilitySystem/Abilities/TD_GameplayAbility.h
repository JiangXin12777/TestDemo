#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TD_GameplayAbility.generated.h"

class UTD_AbilitySystemComponent;

UCLASS()
class UTD_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend UTD_AbilitySystemComponent;
	
public:
	UTD_GameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	UTD_AbilitySystemComponent* GetTDAbilitySystemComponentFromActorInfo() const;

	
};