#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "TD_CharacterBase.generated.h"

/**
 * 角色基类
 */
UCLASS(Config = Game)
class ATD_CharacterBase
	//: public AModularCharacter
	//: public AAlsCharacter
	: public ACharacter
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATD_CharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "TD|Character")
	UTD_AbilitySystemComponent* GetTDAbilitySystemComponent() const;

protected:
	// Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End IAbilitySystemInterface
};