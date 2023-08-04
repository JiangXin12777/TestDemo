#include "GameMode/TD_GameModeBase.h"

#include "GameMode/TD_CharacterBase.h"
#include "GameMode/TD_GameStateBase.h"
#include "GameMode/TD_HUDBase.h"
#include "GameMode/TD_PlayerControllerBase.h"
#include "GameMode/TD_PlayerStateBase.h"

ATD_GameModeBase::ATD_GameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ATD_GameStateBase::StaticClass();
	PlayerControllerClass = ATD_PlayerControllerBase::StaticClass();
	PlayerStateClass = ATD_PlayerStateBase::StaticClass();
	DefaultPawnClass = ATD_CharacterBase::StaticClass();
	HUDClass = ATD_HUDBase::StaticClass();
}
