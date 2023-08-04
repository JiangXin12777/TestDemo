#include "Animation/TD_AnimInstance.h"

#include "GameMode/TD_CharacterBase.h"

UTD_AnimInstance::UTD_AnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTD_AnimInstance::BeginDestroy()
{
	Super::BeginDestroy();
}

void UTD_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UTD_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UTD_AnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void UTD_AnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void UTD_AnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void UTD_AnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}
