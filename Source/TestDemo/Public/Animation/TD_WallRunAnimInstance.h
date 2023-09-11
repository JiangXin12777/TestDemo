#pragma once

#include "CoreMinimal.h"
#include "GameMode/TD_CharacterBase.h"
#include "Movement/TD_MovementType.h"
#include "TD_WallRunAnimInstance.generated.h"

class UTD_WallRunComponent;
class ATD_CharacterBase;

/**
 * 动画蓝图基类
 */
UCLASS()
class UTD_WallRunAnimInstance
	: public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run", meta=(AllowPrivateAccess))
	TWeakObjectPtr<UTD_WallRunComponent> WallRunComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run", meta=(AllowPrivateAccess))
	TWeakObjectPtr<ATD_CharacterBase> OwnerCharacter;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run")
	bool IsWallRunning = false;

	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run")
	bool IsWallJumping = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run")
	EWallSide WallSide;

	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run")
	float CurrentSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run")
	float CurrentAnimPlayRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run")
	float CurrentArcAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run")
	float TurnRate;

	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run")
	float WallJumpAnimationSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run")
	float HandCorrectionAdditive;

public:
	UTD_WallRunAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~ Begin UAnimInstance Interface
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativePostEvaluateAnimation() override;
	virtual void NativeUninitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	//~ End UAnimInstance Interface

	void UpdateAnimInstanceProperty();
};