#pragma once

#include "CoreMinimal.h"
#include "GameMode/TD_CharacterBase.h"
#include "TD_MonsterCharacter.generated.h"

/**
 * 怪物基类
 */
UCLASS(Config = Game)
class ATD_MonsterCharacter
	: public ATD_CharacterBase
{
	GENERATED_BODY()
	
public:
	ATD_MonsterCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};