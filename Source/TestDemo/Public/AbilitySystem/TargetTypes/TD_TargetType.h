#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "TD_TargetType.generated.h"

class AActor;
struct FGameplayEventData;
class ATD_CharacterBase;

/**
 * Class that is used to determine targeting for abilities
 * It is meant to be blueprinted to run target logic
 * This does not subclass GameplayAbilityTargetActor because this class is never instanced into the world
 * This can be used as a basis for a game-specific targeting blueprint
 * If your targeting is more complicated you may need to instance into the world once or as a pooled actor
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class UTD_TargetType : public UObject
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UTD_TargetType() {}

	/** Called to determine targets to apply gameplay effects to */
	UFUNCTION(BlueprintNativeEvent, Category = "TD|TargetType")
	void GetTargets(ATD_CharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FGameplayAbilityTargetDataHandle>& OutTargetData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
	virtual void GetTargets_Implementation(ATD_CharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FGameplayAbilityTargetDataHandle>& OutTargetData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const {}
};

/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class UTD_TargetType_UseOwner : public UTD_TargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UTD_TargetType_UseOwner() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ATD_CharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FGameplayAbilityTargetDataHandle>& OutTargetData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class UTD_TargetType_UseEventData : public UTD_TargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UTD_TargetType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ATD_CharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FGameplayAbilityTargetDataHandle>& OutTargetData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};
