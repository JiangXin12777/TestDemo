#pragma once

#include "AbilitySystemComponent.h"
#include "TD_InputLog.h"
#include "Input/TD_InputManagerComponent.h"

struct FTD_InputHandlerHelpers
{
    /** 检查所有者的 ASC 是否通过提供的查询测试。在这种情况下，空查询将被忽略，测试将返回 true。 */
    static bool HasTags(const UTD_InputManagerComponent* Manager, const FGameplayTagQuery& Query)
    {
        if (Query.IsEmpty() || !IsValid(Manager))
            return true;

        const TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = Manager->GetOwnerAbilitySystemComponent();
        checkf(IsValid(AbilitySystemComponent), TEXT("No ASC received from the Input Manager."));
    
        FGameplayTagContainer OutContainer;
        AbilitySystemComponent->GetOwnedGameplayTags(OutContainer);
        return OutContainer.MatchesQuery(Query);
    }
    
    /** 通过管理器的 ASC 发送游戏事件。 */
    static int32 SendGameplayEvent(const UTD_InputManagerComponent* Manager, const FGameplayTag& GameplayEventTag, const FInputActionValue& Value, const UInputAction* InputAction, const FString Context = TEXT("Local Execution"))
    {
        int32 Activations = 0;
        if (!IsValid(Manager))
            return Activations;
        
        const TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = Manager->GetOwnerAbilitySystemComponent();
    
        if (ensureMsgf(IsValid(AbilitySystemComponent), TEXT("No ASC received from the Input Manager.")) &&
            ensureMsgf(GameplayEventTag.IsValid(), TEXT("The Gameplay Event Tag must be valid.")))
        {
            FGameplayEventData* Payload = new FGameplayEventData();
            Payload->Instigator = Manager->GetOwner();
            Payload->Target = Manager->GetOwner();
            Payload->EventTag = GameplayEventTag;
            Payload->EventMagnitude = Value.GetMagnitude();
            Payload->OptionalObject = InputAction;

            Activations = AbilitySystemComponent->HandleGameplayEvent(GameplayEventTag, Payload);
            
            UE_LOG(TD_Log_InputHandler, Verbose, TEXT("[%s] Action %s triggered event %s with %d activations. Context: %s"),
                *GetNameSafe(Manager->GetOwner()), *GetNameSafe(InputAction), *GameplayEventTag.ToString(), Activations, *Context);
        }

        return Activations;
    }

    /** 按类别中断技能。 */
    static void InterruptAbilityByClass(const UTD_InputManagerComponent* Manager, const UInputAction* InputAction, const TSubclassOf<UGameplayAbility>& AbilityClass)
    {
        if (!IsValid(Manager))
            return;
        
        const TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = Manager->GetOwnerAbilitySystemComponent();
        if (ensure(IsValid(AbilitySystemComponent)))
        {
            const FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromClass(AbilityClass);
            if (Spec != nullptr && Spec->Handle.IsValid())
            {
                UE_LOG(TD_Log_InputHandler, Verbose, TEXT("[%s] Action %s is interrupting ability with class %s."),
                    *GetNameSafe(Manager->GetOwner()), *GetNameSafe(InputAction), *GetNameSafe(AbilityClass));
                
                AbilitySystemComponent->CancelAbilityHandle(Spec->Handle);
            }
        }
    }

    /** 通过游戏标签中断能力。 */
    static void InterruptAbilityByTags(const UTD_InputManagerComponent* Manager, const UInputAction* InputAction, const FGameplayTagContainer& AbilityTags, const FGameplayTagContainer& CancelFilterTags)
    {
        if (!IsValid(Manager))
            return;
        
        const TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = Manager->GetOwnerAbilitySystemComponent();
        if (ensure(IsValid(AbilitySystemComponent)))
        {
            UE_LOG(TD_Log_InputHandler, Verbose, TEXT("[%s] Action %s is interrupting abilities with tags %s."),
                *GetNameSafe(Manager->GetOwner()), *GetNameSafe(InputAction), *AbilityTags.ToStringSimple());

            AbilitySystemComponent->CancelAbilities(&AbilityTags, &CancelFilterTags);
        }
    }

    /** 通过输入 ID 中断能力。 */
    static void InterruptAbilityByInputID(const UTD_InputManagerComponent* Manager, const UInputAction* InputAction, const int32 InputID)
    {
        if (!IsValid(Manager))
            return;
        
        const TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = Manager->GetOwnerAbilitySystemComponent();
        if (ensure(IsValid(AbilitySystemComponent)))
        {
            UE_LOG(TD_Log_InputHandler, Verbose, TEXT("[%s] Action %s is interrupting abilities with ID %d."),
                *GetNameSafe(Manager->GetOwner()), *GetNameSafe(InputAction), InputID);
			
            AbilitySystemComponent->AbilityLocalInputReleased(InputID);
        }
    }
};
