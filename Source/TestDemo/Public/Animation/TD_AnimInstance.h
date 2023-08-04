#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TD_AnimInstance.generated.h"

class ATD_CharacterBase;
class UTD_CharacterMovementComponent;

/**
 * 动画蓝图基类
 */
UCLASS()
class UTD_AnimInstance
	: public UAnimInstance
{
	GENERATED_BODY()

public:
	UTD_AnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~ Begin UObject Interface
	virtual void BeginDestroy() override;
	//~ End UObject Interface
	
	//~ Begin UAnimInstance Interface
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativePostEvaluateAnimation() override;
	virtual void NativeUninitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	//~ End UAnimInstance Interface
};