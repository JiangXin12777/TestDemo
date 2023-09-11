#include "Animation/TD_WallRunAnimInstance.h"

#include "Movement/TD_WallRunComponent.h"

UTD_WallRunAnimInstance::UTD_WallRunAnimInstance(const FObjectInitializer& ObjectInitializer)
{
}

void UTD_WallRunAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UTD_WallRunAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter.IsValid() && WallRunComponent.IsValid())
	{
		// 更新墙跑相关的参数
		UpdateAnimInstanceProperty();
	}
	else
	{
		OwnerCharacter = Cast<ATD_CharacterBase>(TryGetPawnOwner());
		if (OwnerCharacter.IsValid())
		{
			WallRunComponent = OwnerCharacter->GetWallRunComponent();
		}
	}
}

void UTD_WallRunAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void UTD_WallRunAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void UTD_WallRunAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void UTD_WallRunAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UTD_WallRunAnimInstance::UpdateAnimInstanceProperty()
{
	//从角色收集与动画相关的信息。
	IsWallRunning = WallRunComponent->bWallRunning;
	WallSide = WallRunComponent->WallSide;
	CurrentArcAngle = WallRunComponent->CurrentArcAngle;
	IsWallJumping = WallRunComponent->bWallJumping;
	TurnRate = WallRunComponent->CurrentTurnRate;
	CurrentSpeed = OwnerCharacter->GetVelocity().Length();

	//根据角色移动的速度设置运行动画的播放速度。800移动速度=默认动画速度。
	//动画只能以+/- 15%的速度移动，以防止它在非常高或非常低的移动速度下看起来很荒谬。
	CurrentAnimPlayRate = FMath::GetMappedRangeValueClamped(FVector2D(680.f, 920.f), FVector2D(0.85f, 1.15f), CurrentSpeed);


	//跳墙动画的速度相对于期望的跳墙速度而增加。
	//这改善了快速跳墙的外观，因为动画将更好地匹配快速移动。
	//底部值是943，因为这是默认跳墙速度的矢量长度。
	FVector WallJumpSpeed(0.f, WallRunComponent->WallJumpHorizontalVelocity, WallRunComponent->WallJumpVerticalVelocity);
	WallJumpAnimationSpeed = FMath::GetMappedRangeValueClamped(FVector2D(943.f, 1500.f), FVector2D(1.f, 1.5f), WallJumpSpeed.Length());


	//计算一个alpha值(0-1)，该值将用于手部修正附加姿态。
	//这个附加的姿势是一个辅助，以帮助手的位置保持一致，在弧壁运行混合空间。
	//如果不这样做，手将夹在混合空间的0到45度之间的墙壁上。
	float TargetHandCorrectionAdditive =
		CurrentArcAngle >= 15.f
			? FMath::GetMappedRangeValueClamped(FVector2D(30.f, 45.f), FVector2D(1.f, 0.f), CurrentArcAngle)
			: FMath::GetMappedRangeValueClamped(FVector2D(0.f, 30.f), FVector2D(0.f, 1.f), CurrentArcAngle);
	HandCorrectionAdditive =
		FMath::FInterpTo(HandCorrectionAdditive, TargetHandCorrectionAdditive, OwnerCharacter->GetWorld()->GetDeltaSeconds(), 7.f);
}
