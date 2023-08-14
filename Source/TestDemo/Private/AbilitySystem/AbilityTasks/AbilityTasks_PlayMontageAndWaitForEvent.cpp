#include "AbilitySystem/AbilityTasks/AbilityTasks_PlayMontageAndWaitForEvent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"

UAbilityTasks_PlayMontageAndWaitForEvent::UAbilityTasks_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Rate(1.f)
	, StartSection(NAME_None)
	, bStopWhenAbilityEnds(true)
{
}

UAbilityTasks_PlayMontageAndWaitForEvent* UAbilityTasks_PlayMontageAndWaitForEvent::CreatePlayMontageAndWaitProxy(
	UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay,
	FGameplayTagContainer InEventTags, float Rate, FName StartSection, bool bStopWhenAbilityEnds,
	float AnimRootMotionTranslationScale, float StartTimeSeconds)
{
	if (!OwningAbility)
		return nullptr;
	
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UAbilityTasks_PlayMontageAndWaitForEvent* MyObj = NewAbilityTask<UAbilityTasks_PlayMontageAndWaitForEvent>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->EventTags = InEventTags;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->StartTimeSeconds = StartTimeSeconds;
	
	return MyObj;
}

void UAbilityTasks_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
	{
		if (Montage == MontageToPlay)
		{
			if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
			{
				ASC->ClearAnimatingAbility(Ability);
			}

			// Reset AnimRootMotionTranslationScale
			ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
			if (Character && (Character->GetLocalRole() == ROLE_Authority ||
							  (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
			{
				Character->SetAnimRootMotionTranslationScale(1.f);
			}

		}
	}

	if (bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UAbilityTasks_PlayMontageAndWaitForEvent::OnMontageCancel()
{
	if (StopPlayingMontage())
	{
		// Let the BP handle the interrupt as well
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UAbilityTasks_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	EndTask();
}

void UAbilityTasks_PlayMontageAndWaitForEvent::Activate()
{
	Super::Activate();

	if (Ability == nullptr)
		return;

	bool bPlayedMontage = false;

	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			DamageEventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(
				FGameplayTagContainer(),
				FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UAbilityTasks_PlayMontageAndWaitForEvent::OnDamageGameplayEvent));
			
			if (ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection, StartTimeSeconds) > 0.f)
			{
				// Playing a montage could potentially fire off a callback into game code which could kill this ability! Early out if we are  pending kill.
				if (ShouldBroadcastAbilityTaskDelegates() == false)
				{
					return;
				}

				CancelHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UAbilityTasks_PlayMontageAndWaitForEvent::OnMontageCancel);

				BlendingOutDelegate.BindUObject(this, &UAbilityTasks_PlayMontageAndWaitForEvent::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UAbilityTasks_PlayMontageAndWaitForEvent::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
				if (Character && (Character->GetLocalRole() == ROLE_Authority ||
								  (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
				{
					Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}

				bPlayedMontage = true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UAbilityTask_PlayMontageAndWait call to PlayMontage failed!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UAbilityTask_PlayMontageAndWait called on invalid AbilitySystemComponent"));
	}

	if (!bPlayedMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAbilityTask_PlayMontageAndWait called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay),*InstanceName.ToString());
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	SetWaitingOnAvatar();
}

void UAbilityTasks_PlayMontageAndWaitForEvent::ExternalCancel()
{
	OnMontageCancel();

	Super::ExternalCancel();
}

FString UAbilityTasks_PlayMontageAndWaitForEvent::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? ToRawPtr(MontageToPlay) : AnimInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(TEXT("PlayMontageAndWait. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}

void UAbilityTasks_PlayMontageAndWaitForEvent::OnDestroy(bool AbilityEnded)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, DamageEventHandle);
	}

	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelHandle);
		if (AbilityEnded && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}
	
	Super::OnDestroy(AbilityEnded);
}

bool UAbilityTasks_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
	if (Ability == nullptr)
		return false;

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (ActorInfo == nullptr)
		return false;

	const UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
		return false;

	// 检查蒙太奇是否仍在播放
	// 该能力会被中断，在这种情况下我们应该自动停止蒙太奇
	if (AbilitySystemComponent.IsValid())
	{
		if (AbilitySystemComponent->GetAnimatingAbility() == Ability
			&& AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
		{
			// 取消绑定代表，这样他们就不会被呼叫
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			AbilitySystemComponent->CurrentMontageStop();
			return true;
		}
	}

	return false;
}

void UAbilityTasks_PlayMontageAndWaitForEvent::OnDamageGameplayEvent(FGameplayTag InGameplayTag,
                                                                     const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData EventData = *Payload;
		EventData.EventTag = InGameplayTag;

		OnDamageEventReceived.Broadcast(InGameplayTag, EventData);
	}
}
