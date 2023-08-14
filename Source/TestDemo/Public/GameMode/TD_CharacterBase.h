#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularCharacter.h"
#include "TD_CharacterBase.generated.h"

class UTD_AttributeSet;
class UTD_AbilitySystemComponent;

/**
 * 角色基类
 */
UCLASS(Config = Game)
class ATD_CharacterBase
	: public AModularCharacter
	//: public AAlsCharacter
	//: public ACharacter
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** !技能组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Character|Ability", meta = (AllowPrivateAccess))
	TObjectPtr<UTD_AbilitySystemComponent> AbilitySystemComponent;

	/** !属性集合 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Character|Ability", meta = (AllowPrivateAccess))
	TObjectPtr<UTD_AttributeSet> AttributeSet;

public:
	ATD_CharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 获取 TD 技能组件 */
	UTD_AbilitySystemComponent* GetTDAbilitySystemComponent() const;

	/** 获取 TD 属性集合 */
	UTD_AttributeSet* GetTDAttributeSet() const;

protected:
	// Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End IAbilitySystemInterface

	// ~APawn Interface
	virtual void OnRep_PlayerState() override;
	// ~APawn Interface

	// ~Begin Actor Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginDestroy() override;
	// ~End  Actor Interface

	void InitializeAbilitySystem();
};