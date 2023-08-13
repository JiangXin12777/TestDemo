#include "AbilitySystem/Abilities/TD_AbilityType.h"

bool FTD_GameplayEffectContainerSpec::HasValidEffects() const
{
	return TargetGameplayEffectSpecs.Num() > 0;
}

bool FTD_GameplayEffectContainerSpec::HasValidTargets() const
{
	return TargetData.Num() > 0;
}

void FTD_GameplayEffectContainerSpec::AddTargets(const TArray<FGameplayAbilityTargetDataHandle>& InTargetData,
	const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
	for (const FGameplayAbilityTargetDataHandle& TD : InTargetData)
	{
		TargetData.Append(TD);
	}

	for (const FHitResult& HitResult : HitResults)
	{
		FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		TargetData.Add(NewData);
	}

	if (TargetActors.Num() > 0)
	{
		FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
		NewData->TargetActorArray.Append(TargetActors);
		TargetData.Add(NewData);
	}
}

void FTD_GameplayEffectContainerSpec::ClearTargets()
{
	TargetData.Clear();
}
