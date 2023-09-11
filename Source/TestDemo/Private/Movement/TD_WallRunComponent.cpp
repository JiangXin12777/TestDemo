#include "Movement/TD_WallRunComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Log/TD_Log.h"

UTD_WallRunComponent::UTD_WallRunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_Visibility);
}

bool UTD_WallRunComponent::StartWallJump()
{
	if (bWallRunning)
	{
		StopWallRun(MOVE_Falling, 0.1f);
		
		//通过以期望的方向/速度发射角色来执行跳跃。
		//方向是通过获取当前的AverageWallNormal(它直接指向远离墙壁的地方)并根据所提供的WallJumpAngle旋转它来确定的。
		//该角度被映射到一个反向范围(从90->0到0->90)，因为所提供的角度假设0 =向前，90 =垂直于墙壁，但我们开始使用的墙壁法线已经垂直于墙壁。
		//如果已知90度，我们不旋转矢量。
		//z -速度单独作为参数提供。

		// 跳墙角度映射
		float WallJumpAngleMap = UKismetMathLibrary::MapRangeClamped(WallJumpAngle, 90.f, 0.f, 0.f, WallSide == EWallSide::EWS_Left ? -90.f : 90.f);
		// 跳墙方向计算
		FVector LauncherCharacterVector = UKismetMathLibrary::Quat_RotateVector(FRotator(0.f, WallJumpAngleMap, 0.f).Quaternion(), AverageWallNormal);
		LauncherCharacterVector *= WallJumpHorizontalVelocity;

		// 打印结果
		FString Log = FString::Printf(TEXT("WallJumpAngleMap=[%f]--LauncherCharacterVector=[%s]"), WallJumpAngleMap, *LauncherCharacterVector.ToString());
		PrintLog(Log, FColor::Red, 2.f);

		if (OwnerCharacter.IsValid())
		{
			// 弹起角色
			OwnerCharacter->LaunchCharacter(FVector(LauncherCharacterVector.X, LauncherCharacterVector.Y, WallJumpVerticalVelocity), true, true);

			// 记录 WallJumping 状态
			bWallJumping = true;

			//禁用OrientRotationToMovement，以便我们可以手动控制字符旋转，然后删除所有空气控制。
			//然后我们在短暂延迟后授予空中控制(如果通过参数允许)。
			//这种延迟让玩家能够在覆盖跳跃方向之前确定跳跃方向和速度，确保与初始跳跃方向/速度保持一致，而不管玩家输入什么。
			OwnerMovementComponent->bOrientRotationToMovement = false;
			OwnerMovementComponent->AirControl = 0.f;
			DelayedAirControl();

			// 通知角色移动模式改变了
			//OwnerCharacter->SetSpecialMovementMode(AlsSpecialMovementModeTags::SPM_WallJump);
			return true;
		}
		else
		{
			UE_LOG(TD_CharacterMovement, Warning, TEXT("UTD_WallRunComponent::OwnerCharacter is nullptr!!!"));
			return false;
		}
	}
	return false;
}

void UTD_WallRunComponent::StopWallJump()
{
	// 结束WallJumping状态，并将角色的移动设置重置为原始值。
	bWallJumping = false;
	OwnerMovementComponent->bOrientRotationToMovement = bDefaultOrientRotation;
	OwnerMovementComponent->AirControl = DefaultAirControl;
	OwnerMovementComponent->MaxAcceleration = DefaultMaxAcceleration;

	// 如果WallJumpAirControlTimer(在一定的延迟后允许空中控制)仍然在倒计时，停止它。
	// 这可以防止计时器在跳跃已经结束后错误地应用WallJump移动设置。
	if (UKismetSystemLibrary::K2_IsTimerActiveHandle(GetWorld(), WallJumpAirControlTimer))
	{
		UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(GetWorld(), WallJumpAirControlTimer);
	};

	// 通知角色移动模式改变了
	//OwnerCharacter->SetSpecialMovementMode(AlsSpecialMovementModeTags::None);
	
	//UpdateRotationALS();
}

bool UTD_WallRunComponent::StartWallRun(EWallSide Side)
{
	if (bWallRunningAvailable && OwnerMovementComponent.IsValid())
	{
		//只有在以下情况下才允许跑墙:

		//玩家角色在之前的Tick中朝着墙壁的方向移动(我们使用之前tick的速度，因为如果墙壁探测轨迹在角色胶囊撞到墙壁的同一tick中被触发，当前的速度反映了撞击后的速度，这对我们在这里没有用处)
		//玩家在墙的方向上提供移动输入
		//如果点积的结果是负的，则字符(或其输入向量)正在向墙壁移动。如果是正的，它们在远离墙。
		bool A = UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::Normal(PreviousVelocity, 0.0001), AverageWallNormal) < 0.f;
		bool B = UKismetMathLibrary::Dot_VectorVector(OwnerMovementComponent->GetLastInputVector(), AverageWallNormal) < 0.f;

		if (A || B)
		{
			// 如果当前正在跳墙，请结束跳墙。
			if (bWallJumping)
			{
				StopWallJump();
			}
			// 更新跟踪跑墙状态的值。
			bWallRunning = true;
			WallSide = Side;

			// 设置跑墙的移动属性设置
			FVector PreVelocity = OwnerCharacter->GetVelocity();
			OwnerMovementComponent->MaxCustomMovementSpeed = WallRunSpeed;
			OwnerMovementComponent->MaxFlySpeed = WallRunSpeed;
			OwnerMovementComponent->bOrientRotationToMovement = false;
			OwnerMovementComponent->MaxAcceleration = WallRunAcceleration;
			OwnerMovementComponent->BrakingDecelerationFlying = WallRunBrakingDeceleration;
			OwnerMovementComponent->Velocity = PreVelocity;

			OwnerMovementComponent->SetMovementMode(MOVE_Flying);
			// 重置持续时间，跟踪当前wall运行的活动时间。这是用来控制弧度的跑墙。
			CurrentWallRunDuration = 0.f;

			// 初始化当前旋转为“PreviousRotation”，以摆脱来自过去墙壁运行的任何过时数据。
			PreviousRotation = OwnerCharacter->GetActorRotation();

			// 如果我们想让跑墙在一定时间后自动结束，启动倒计时将结束它。
			if (WallRunDurationLimit >= 0.f)
			{
				StartDurationCountdown();
			}

			// 启动一个计时器，以特定的时间间隔跟踪玩家的速度，以确定跑墙是否卡住了什么东西。
			StartMonitoringSpeed();

			// 清除角色上的任何现有速度，这样墙壁上的奔跑就能始终保持一致(否则，奔跑的距离/高度可能会受到角色开始奔跑时的速度的影响)。
			// 这也可以防止旧的速度值对弧角动画混合产生负面影响，这是基于速度的。
			OwnerMovementComponent->Velocity = FVector(0.f, 0.f, 0.f);

			// 设置新的CustomMode
			//OwnerCharacter->SetSpecialMovementMode(AlsSpecialMovementModeTags::SPM_WallRun);
			return true;
		}
	}

	return false;
}

void UTD_WallRunComponent::WallRunTick()
{
	if (bWallRunning)
	{
		//让角色靠近墙壁(但不要太近)。
		HugWall();

		//将移动输入添加到角色所需的墙壁运行方向。
		//我们通过寻找AverageWallNormal和向上/向下向量(向上或向下取决于墙位于角色的哪一侧)的叉乘来获得移动方向。
		//缩放值设置得非常高，使玩家手动输入的效果相形见绌。这种方法可能看起来很奇怪，但它比在角色BP中添加逻辑以在适当的时间过滤玩家的移动输入要简单得多(这将使将系统合并到其他角色中更加复杂)。

		FVector CrossValue = UKismetMathLibrary::Cross_VectorVector(AverageWallNormal, FVector(0.f, 0.f, WallSide == EWallSide::EWS_Left ? 1.f : -1.f));

		//通过在移动输入中添加z分量，在墙壁上添加弧形。
		//z分量的定义方法是取当前的墙壁运行持续时间，将其映射到一段时间内1到-2的范围，然后乘以全局ArcAmount标量。
		//结果是弧线从1(向上)开始，然后随着时间的推移平滑过渡到-2(向下)。
		FVector Dir = FVector(CrossValue.X, CrossValue.Y,
							  UKismetMathLibrary::MapRangeClamped(CurrentWallRunDuration, 0.f, 2.5f, 1.f, -2.f) * ArcAmount);
		OwnerCharacter->AddMovementInput(Dir, 10000, false);


		//平滑地旋转角色，使其面向墙跑的方向。
		//RInterp速度与角色的速度成比例地增加，因此速度越快=转弯越快。这使得高速转弯看起来更好，也有助于系统更好地跟踪高速弯道。

		float InterpSpeed = UKismetMathLibrary::MapRangeClamped(OwnerCharacter->GetVelocity().Size(), 800.f, 1000.f, 8.f, 16.f);
		FRotator Rot = UKismetMathLibrary::RInterpTo(OwnerCharacter->GetActorRotation(), UKismetMathLibrary::Conv_VectorToRotator(CrossValue),
													 GetWorld()->GetDeltaSeconds(), InterpSpeed);
		OwnerCharacter->SetActorRotation(Rot);


		//通过将浮点数乘以World Delta来跟踪当前wall运行的活动时间。
		CurrentWallRunDuration += GetWorld()->GetDeltaSeconds();

		//检查墙上是否有足够的垂直空间让玩家继续在墙上奔跑。
		//如果玩家正在接近一堵墙的顶部，这将限制他们的垂直移动，这样他们就不会从墙的顶部跑掉。
		//然后我们检查字符底部附近是否有足够的垂直空间。
		//如果胶囊的底部开始悬挂在墙壁的底部，跑步将会结束。
		TopHeightCheck();
		BottomHeightCheck(WallSide);

		//更新任何仅用于动画目的的值。
		UpdateAnimationValues();
	}
}

void UTD_WallRunComponent::StopWallRun(EMovementMode NewMovementMode, float TemporarilyDisableWallRunSeconds)
{
	//将IsWallRunning(状态跟踪器)设置为false，并恢复角色原始的移动设置。
	bWallRunning = false;
	OwnerMovementComponent->MaxFlySpeed = DefaultMaxFlySpeed;
	OwnerMovementComponent->bOrientRotationToMovement = bDefaultOrientRotation;
	OwnerMovementComponent->MaxAcceleration = DefaultMaxAcceleration;
	OwnerMovementComponent->BrakingDecelerationFlying = DefaultBrakingDecelerationFlying;
	//设置角色使用所需的移动模式。之所以这么做，是因为我们希望玩家进入“行走”或“坠落”状态，这取决于跑墙的方式。
	OwnerMovementComponent->SetMovementMode(NewMovementMode);
	//取消最大墙壁运行时间倒计时，如果它目前处于活动状态，并停止监控角色的速度。
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(GetWorld(), DurationCountdownTimer);
	//如果需要，暂时禁用墙壁运行。
	StopMonitoringSpeed();
	if (TemporarilyDisableWallRunSeconds > 0.f)
	{
		TemporarilyDisableWalkRunning(TemporarilyDisableWallRunSeconds);
	}

	//OwnerCharacter->SetSpecialMovementMode(AlsSpecialMovementModeTags::None);
	
	//如果使用ALS，告诉它角色的旋转应该是什么。
	//UpdateRotationALS();
}

void UTD_WallRunComponent::HugWall()
{
	//将角色推向或远离墙壁，目标是在墙壁和角色之间保持1个单位。这样做有两个原因:
	//1. 如果角色离墙太远，墙就会消失，跑墙就会结束
	//2. 如果角色离墙壁太近，就会导致角色移动方式发生意外变化(我认为这是由于胶囊撞到墙壁造成的)
	//DistanceFromWall变量在DetectWall函数期间被记录。
	//它是墙壁和角色胶囊之间的空间量。通过从1中减去它，我们发现我们需要移动角色多远才能在胶囊和墙壁之间保持一个单位的间隙。

	OwnerCharacter->AddActorWorldOffset(AverageWallNormal * (1.f - DistanceFromWall), true);
}

void UTD_WallRunComponent::WallJumpTick()
{
	// 如果目前跳墙，平稳旋转角色面向他们目前移动的方向。
	// 这是用来代替OrientRotationToMovement的，因为OrientRotationToMovement旋转角色的方向是移动输入应用到角色，而不是角色的实际移动方向。
	if (bWallJumping)
	{
		FRotator CharacterRorator = OwnerCharacter->GetActorRotation();
		FRotator TargetRotator = UKismetMathLibrary::Conv_VectorToRotator(OwnerCharacter->GetVelocity());
		OwnerCharacter->SetActorRotation(UKismetMathLibrary::RInterpTo(CharacterRorator,
																	   FRotator(0.f, TargetRotator.Yaw, 0.f),
																	   GetWorld()->GetDeltaSeconds(),
																	   5.0f));
	}
}

void UTD_WallRunComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	InitCharacterInformation();
	PrimaryTraceSize = 0.85f * CapsuleRadius;

	OwnerCharacter->MovementModeChangedDelegate.AddDynamic(this, &ThisClass::OnMovementModeChangedEvent);

	if (OwnerMovementComponent->MovementMode == MOVE_Falling)
	{
		SetComponentTickEnabled(true);
	}

	OwnerCharacter->OnActorHit.AddDynamic(this, &ThisClass::OnActorHitEvent_CheckFloor);
}

void UTD_WallRunComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerCharacter.IsValid())
	{
		Update();
	}
}

void UTD_WallRunComponent::Update()
{
	//沿着当前墙壁的方向进行追踪以确定我们是否还在上面。
	if (bWallRunning)
	{
		switch (WallSide)
		{
		case EWallSide::EWS_Left:
			DetectWall(EWallSide::EWS_Left);
			break;
		case EWallSide::EWS_Right:
			DetectWall(EWallSide::EWS_Right);
			break;
		}
	}
	else
	{
		//如果我们不是在墙上跑，执行跳墙更新(如果是跳墙)，然后寻找玩家两侧的墙，开始在墙上跑。
		//我们首先检查玩家的右侧，如果没有检测到墙，我们再检查左侧。
		WallJumpTick();
		DetectWall(EWallSide::EWS_Right);
		if (!bWallRunning)
		{
			DetectWall(EWallSide::EWS_Left);
		}
	}

	//我们在墙上，执行Tick动画更新即可
	WallRunTick();

	//记录玩家在Tick结束时的速度。这样我们就可以知道玩家在触碰墙壁之前是如何移动的。
	PreviousVelocity = OwnerCharacter->GetVelocity();
}

void UTD_WallRunComponent::UpdateAnimationValues()
{
	//获取墙运行弧的当前角度。求出玩家的速度与Z设为0时相同速度的点积。
	//这样做时，负值会被翻转为正数，因此最终值乘以原始Z值的符号(1或-1)。
	FVector VelocityNormalize = UKismetMathLibrary::Normal(OwnerCharacter->GetVelocity());
	float DotValue = UKismetMathLibrary::Dot_VectorVector(VelocityNormalize,
														  FVector(VelocityNormalize.X, VelocityNormalize.Y, 0.f));
	float ACosd = UKismetMathLibrary::Acos(DotValue);
	CurrentArcAngle = ACosd * UKismetMathLibrary::SignOfFloat(VelocityNormalize.Z);

	//求出拐弯的速度并将其映射到0-1的范围内。
	//为此，我们获得角色当前和之前旋转的delta值，将其映射到-1到1的范围内，根据墙壁的哪一边切换符号(这样外部角落总是得到一个正值)，
	//然后将值固定，使其不能低于0，因为我们对内部角落的转弯速率不感兴趣。
	//然后对转换率浮动进行插值，使其值变化平滑。

	FRotator CharacterRotator = OwnerCharacter->GetActorRotation();
	FRotator DeltaRotNormal = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotator, PreviousRotation); //归一化的delta旋转值

	FVector RotatorToVector = UKismetMathLibrary::Conv_RotatorToVector(DeltaRotNormal);
	FVector DeltaRotatorToVector = RotatorToVector / GetWorld()->GetDeltaSeconds();
	float MapClamp = UKismetMathLibrary::MapRangeClamped(DeltaRotatorToVector.Y, -10.f, 10.f, -1.f, 1.f);

	MapClamp = MapClamp * (WallSide == EWallSide::EWS_Left ? -1.f : 1.f);
	MapClamp = UKismetMathLibrary::Max(MapClamp, 0.f);

	CurrentTurnRate = UKismetMathLibrary::FInterpTo(CurrentTurnRate, MapClamp, GetWorld()->GetDeltaSeconds(), 10.f);
	PreviousRotation = CharacterRotator;
}

void UTD_WallRunComponent::InitCharacterInformation()
{
	if (ensure(OwnerCharacter.Get()) && OwnerCharacter->IsValidLowLevel())
	{
		OwnerMovementComponent = OwnerCharacter->GetCharacterMovement();
		CapsuleRadius = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
		DefaultMaxFlySpeed = OwnerMovementComponent->MaxFlySpeed;
		bDefaultOrientRotation = OwnerMovementComponent->bOrientRotationToMovement;
		DefaultMaxAcceleration = OwnerMovementComponent->MaxAcceleration;
		DefaultBrakingDecelerationFlying = OwnerMovementComponent->BrakingDecelerationFlying;
		DefaultAirControl = OwnerMovementComponent->AirControl;
	}
}

void UTD_WallRunComponent::OnMovementModeChangedEvent(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	auto CheckWallRunningTick = [&]()
	{
		if (IsComponentTickEnabled())
		{
			SetComponentTickEnabled(false);
		}

		//如果我们正在跳墙，结束跳墙。(移动模式的改变表明玩家可能在跳跃后撞到了地面。)
		if (bWallJumping)
		{
			StopWallJump();
		}
	};


	//当角色的移动模式改变时，根据模式启用或禁用该组件的标记。
	//当角色进入坠落状态时，tick被启用，这样它就可以开始寻找可以在上面运行的墙壁。
	//当任何其他模式被激活时(飞行除外)，Tick被禁用。
	//飞行的处理方式不同，因为这是在墙上奔跑时使用的模式。
	//如果飞行开启Tick，但角色没有在墙上跑，我们禁用组件的tick，因为这表明在墙上跑之外的其他游戏功能正在使用飞行模式，我们不想干扰它
	switch (OwnerMovementComponent->MovementMode)
	{
	case MOVE_Falling:
		{
			SetComponentTickEnabled(true);
			PreviousVelocity = OwnerCharacter->GetVelocity(); //通过将PreviousVelocity变量重新初始化为当前速度来清除任何可能的旧数据。
			break;
		}
	case MOVE_None:
		CheckWallRunningTick();
		break;
	case MOVE_Walking:
		CheckWallRunningTick();
		break;
	case MOVE_NavWalking:
		CheckWallRunningTick();
		break;
	case MOVE_Swimming:
		CheckWallRunningTick();
		break;
	case MOVE_Flying:
		if (!bWallRunning)
		{
			CheckWallRunningTick();
		}
		break;
	case MOVE_Custom:
		if (!bWallRunning)
		{
			CheckWallRunningTick();
		}
		break;
	case MOVE_MAX:
		break;
	default: ;
	}

	//如果玩家是在空中诞生的(因此处于坠落状态)，激活组件的tick。这通常是在移动模式变为下落状态时处理的，但如果角色是在下落时生成的，则必须单独处理。
	if (OwnerMovementComponent->MovementMode == MOVE_Falling)
	{
		SetComponentTickEnabled(true);
	}
}

void UTD_WallRunComponent::OnActorHitEvent_CheckFloor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit)
{
	//当击中一个在墙上奔跑的角色时，检查该击中是否在一个可行走的表面，以及是否在角色脚下。如果是，结束墙跑，降落在地板上。
	if (OwnerMovementComponent->IsWalkable(Hit))
	{
		if (bWallRunning)
		{
			//GetScaledCapsuleHalfHeight_WithoutHemisphere 获得没有半球的缩放胶囊半高
			float Height = OwnerCharacter->GetActorLocation().Z - OwnerCharacter->GetCapsuleComponent()->
																				  GetScaledCapsuleHalfHeight_WithoutHemisphere();
			if (Hit.ImpactPoint.Z <= Height)
			{
				StopWallRun(MOVE_Walking, 0.0f);
			}
		}
		else
		{
			//如果角色落在地板上，墙壁运行暂时被禁用，立即重新启用它，并停止计时器，在延迟后会这样做。降落在地板上可以立即消除任何对墙跑的限制，让玩家立即开始再次墙跑
			if (UKismetSystemLibrary::K2_IsTimerActiveHandle(GetWorld(), DisabledWallRunTimer))
			{
				//清除计时
				UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(GetWorld(), DisabledWallRunTimer);

				AllowWallRunning(); //重新允许WallRun
			}
		}
	}
}

void UTD_WallRunComponent::DetectWall(EWallSide Side)
{
	//Lambda函数
	//如果我们还没有在墙上跑步，检查一下墙上是否有足够的垂直空间来支持在墙上跑步。如果这样，则重新开始跑墙。
	auto RestartWallRun = [&]()
	{
		if (!bWallRunning)
		{
			// 检测是否可以进行跑墙
			if (BottomHeightCheck(Side) && TopHeightCheck())
			{
				PrintLog(FString(TEXT("StartWallRun")), FColor::Black, 5.f);
				StartWallRun(Side);
			}
		}
	};


	//主射线——查找字符左边或右边的墙(取决于调用函数时提供的是哪一边)。
	//跟踪从角色的位置开始，在距离角色的左边或右边很短的距离结束。
	FHitResult HitResult;
	FVector StartLocation = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * CapsuleRadius * 0.05f;
	float WallRunningValue = (bWallRunning ? 2.f : 0.3f);
	float SideDirValue = (Side == EWallSide::EWS_Left ? -1.f : 1.f);
	FVector EndLocation = StartLocation + (OwnerCharacter->GetActorRightVector() * WallRunningValue * CapsuleRadius * SideDirValue);

	// 腹部的球型检测
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLocation, EndLocation, PrimaryTraceSize, TraceTypeQuery, false, TArray<AActor*>(),
	                                            DrawSphereTrace, HitResult, true, FLinearColor::Black, FLinearColor::Green, 10.f))
	{
		if (IsWallRunning(HitResult.GetActor()))
		{
			PrimaryTraceNormal = HitResult.ImpactNormal;
			AverageWallNormal = PrimaryTraceNormal;
			CurrentWallLocation = HitResult.ImpactPoint;
		
			DistanceFromWall = (HitResult.TraceStart - CurrentWallLocation).Size() - CapsuleRadius;
		
			//二级追踪——寻找玩家前方不远的一堵墙，以识别任何即将到来的转弯/拐角。
			//这条轨迹的位置基于从前一条轨迹收集的信息——它使用前一条轨迹的冲击点和墙壁法线，开始向前并远离墙壁的一小段距离，然后沿与该墙法线相反的方向(朝向墙壁)进行跟踪。
			//末端位置略微向前移动，以便更好地检测内角。
			if (LineTraceRelativeToCapsuleAndWall(PrimaryTraceNormal, CurrentWallLocation, 1.5f, -0.9f, 0.75f, -1.5f, 0.0f, Side, HitResult))
			{
				//如果检测到命中，确保命中的墙符合跑墙的条件。如果墙不符合条件，什么都不做。
				//如果玩家到达这个墙，它将被主跟踪检测到，然后墙跑将结束。
				if (IsWallRunning(HitResult.GetActor()))
				{
					//如果我们已经在墙上运行，请检查主轨迹和此轨迹之间的角度差异。
					//如果拐角太尖，我们可能需要结束墙跑。但是，在这样做之前，我们将执行备份跟踪以确保安全。
					if (bWallRunning)
					{
						// ！内角检测，遇到内拐角后进行转弯
						if (CheckCornerAngle(PrimaryTraceNormal, HitResult.ImpactNormal))
						{
							SecondaryTraceNormal = HitResult.ImpactNormal;
							AverageWallNormal += SecondaryTraceNormal;
							AverageWallNormal = UKismetMathLibrary::Normal(AverageWallNormal, 0.0001);
						
							RestartWallRun(); //重新开始跑墙
						}
						else
						{
							//如果首个辅助跟踪检测到超出所需限制的角落，则执行备份跟踪。
							//该备份痕迹作为故障保险，以防止墙壁几何形状中的小缺口/裂缝/唇形。
							//为了确定第一个跟踪是否碰到了一个应该忽略的小间隙，我们在第一个跟踪前面的一小段距离执行这个跟踪。
							//如果这个轨迹找到一个具有合适角度的曲面，我们就抛出第一个轨迹的结果。
							//如果这一个也检测到一个角度太尖锐的表面(或者如果它根本没有检测到一个表面)，我们继续结束墙跑。
						
							if (LineTraceRelativeToCapsuleAndWall(PrimaryTraceNormal, CurrentWallLocation, 1.5f, -1.f, 0.75f, -1.8f, 0.0f, Side, HitResult))
							{
								if (IsWallRunning(HitResult.GetActor()))
								{
									if (CheckCornerAngle(PrimaryTraceNormal, HitResult.ImpactNormal))
									{
										//辅助射线法线
										SecondaryTraceNormal = HitResult.ImpactNormal;
										AverageWallNormal += SecondaryTraceNormal;
										AverageWallNormal = UKismetMathLibrary::Normal(AverageWallNormal, 0.0001);
							
										RestartWallRun(); //重新开始跑墙
									}
									else
									{
										StopWallRun(MOVE_Falling, 5.f);
									}
								}
								else
								{
									StopWallRun(MOVE_Falling, 1.0f);
								}
							}
							else
							{
								StopWallRun(MOVE_Falling, 1.0f);
							}
						}
					}
					else
					{
						SecondaryTraceNormal = HitResult.ImpactNormal;
						AverageWallNormal += SecondaryTraceNormal;
						AverageWallNormal = UKismetMathLibrary::Normal(AverageWallNormal, 0.0001);

						//重新跑墙
						RestartWallRun(); 
					}
				}
			}
			else
			{
				//辅助射线 （角落射线） - 如果第二射线没有检测到一个表面，则发起一条射线用来检查一个非常尖锐的外部角(~90度)的可能性。为了检测尖角，跟踪开始时略超过拐角所在的位置，然后急剧向后跟踪。

				// ！外角检测
				if (LineTraceRelativeToCapsuleAndWall(PrimaryTraceNormal, CurrentWallLocation, 0.f, -1.5f, 0.f, 2.f, -0.3f, Side,
				                                      HitResult))
				{
					//如果检测到命中，确保命中的墙符合跑墙的条件。
					//如果墙不符合条件，什么都不做。当玩家到达这个墙，它将被主跟踪检测到，然后墙跑将结束。
					if (IsWallRunning(HitResult.GetActor()))
					{
						//如果检测到命中，确保命中的墙符合跑墙的条件。如果墙不符合条件，什么都不做，如果玩家到达这个墙，它将被主跟踪检测到，然后墙跑将结束。
						//如果我们已经在墙上跑了，确定这个轨迹击中的表面是否在所需的角度限制之内。
						//如果不在限制范围内，我们就停止跑墙。
						//如果我们还没有跑墙，继续记录关于墙的信息。
						if (bWallRunning)
						{
							if (CheckCornerAngle(PrimaryTraceNormal, HitResult.ImpactNormal))
							{
								SecondaryTraceNormal = HitResult.ImpactNormal;
								AverageWallNormal += SecondaryTraceNormal;
								AverageWallNormal = UKismetMathLibrary::Normal(AverageWallNormal, 0.0001);
								RestartWallRun(); //重新跑墙
							}
							else
							{
								StopWallRun(MOVE_Falling, 1.0f);
							}
						}
						else
						{
							SecondaryTraceNormal = HitResult.ImpactNormal;
							AverageWallNormal += SecondaryTraceNormal;
							AverageWallNormal = UKismetMathLibrary::Normal(AverageWallNormal, 0.0001);
							RestartWallRun();
						}
					}
				}
			}
		}
		else
		{
			//主射线命中，但是命中的Actor不允许跑墙
			if (bWallRunning)
			{
				StopWallRun(MOVE_Falling, 0.0f);
			}
		}
	}
	else
	{
		//主射线未命中，停止跑墙
		if (bWallRunning)
		{
			StopWallRun(MOVE_Falling, 0.0f);
		}
	}
}

bool UTD_WallRunComponent::LineTraceRelativeToCapsuleAndWall(FVector CurrentWallNormal,
	FVector InCurrentWallLocation, float InitalTraceLength, float StartOffset_Backwards, float StartOffset_AwayFromWall,
	float EndOffset_Backwards, float EndOffset_AwayFromWall, EWallSide Side, FHitResult& HitResult)
{
	FVector StartPointA;
	FVector EndPointA;

	//根据墙的法线和侧面找到 “前进” 方向
	float Dir = (Side == EWallSide::EWS_Left ? -1.f : 1.f);
	FVector ForwardDir = UKismetMathLibrary::Cross_VectorVector(CurrentWallNormal, FVector(0.f, 0.f, Dir));

	//起始位置向前偏移 (positive = backwards)
	float ForwardOffset = CapsuleRadius * StartOffset_Backwards;

	//开始位置横向偏移 (positive = away from wall)
	FVector StartCrosswiseOffset = CurrentWallNormal * CapsuleRadius * StartOffset_AwayFromWall;

	//在应用任何结束位置偏移量之前设置迹线的长度。该值为负值，因此轨迹指向墙壁。
	FVector EndOffset = CurrentWallNormal * InitalTraceLength * CapsuleRadius * -1.f;

	//End position 正向偏移 (positive = backwards)
	FVector EndPointForwarOffset = ForwardDir * CapsuleRadius * EndOffset_Backwards;

	//End position 横向偏移量 (positive = away from wall)
	FVector EndPointCrosswiseOffset = CurrentWallNormal * CapsuleRadius * EndOffset_AwayFromWall;

	//合并, 起始位置
	StartPointA = InCurrentWallLocation + (ForwardDir * ForwardOffset) + StartCrosswiseOffset;

	//合并, 结束位置
	EndPointA = StartPointA + EndOffset + EndPointForwarOffset + EndPointCrosswiseOffset;

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartPointA, EndPointA, TraceTypeQuery, false, TArray<AActor*>(), DrawLineTrace,
											  HitResult,
											  true, FLinearColor::Red, FLinearColor::Green, 2.f))
	{
		return true;
	};

	return false;
}

bool UTD_WallRunComponent::CheckCornerAngle(FVector FirstWallNormal, FVector SecondWallNormal)
{
	//检查所提供的两个表面法线之间的角度有多尖锐。返回true或false，取决于拐角是否在我们的限制范围内。
	double DotValue = UKismetMathLibrary::Dot_VectorVector(FirstWallNormal, SecondWallNormal);

	double ACosValue = UKismetMathLibrary::Acos(DotValue);

	//砍掉小数点，以防止轻微的不准确导致墙跑结束(例如，平墙有时会报告0.02而不是0——如果最大角角设置为0，这将导致墙跑错误地结束。
	//这确实意味着允许的角度比期望的多1度(如果最大角度是45，这将允许进行45.9)，但我认为这是最优雅的解决方案。
	CurrentCornerAngle = UKismetMathLibrary::FTrunc(ACosValue);

	//确定我们计算的是内角还是外角，并返回适当的角度限制。
	//我们通过得到两个表面法线的叉乘(负z =左转，正z =右转)来确定墙壁的转向方向。
	//结合墙在角色的哪一边，我们就能知道我们是在内部还是外部的角落里导航。
	FVector CrossVector = UKismetMathLibrary::Cross_VectorVector(FirstWallNormal, SecondWallNormal);
	float Sign = UKismetMathLibrary::SignOfFloat(CrossVector.Z);
	bool A = (WallSide == EWallSide::EWS_Left && Sign < 0.f);
	bool B = (WallSide == EWallSide::EWS_Right && Sign >= 0.f);
	bool bPickA = A || B;

	float SelectFloatValue = UKismetMathLibrary::SelectFloat(OutsideCornerAngleLimit, InsideCornerAngleLimit, bPickA);
	bool Result = CurrentCornerAngle <= SelectFloatValue;

	return Result;
}

bool UTD_WallRunComponent::IsWallRunning(AActor* WallActor)
{
	// 我们首先检查actor是否有效，以避免在Mesh(不返回actor对象，也不支持actor标记)上运行该检查时出现任何输出错误。
	// 如果它是无效的，并且需要一个标签来运行墙壁，我们返回false，因为Mesh永远不会被标记。
	if (IsValid(WallActor))
	{
		if (WallActor->ActorHasTag(TagForPreventingWallRun))
		{
			return false;
		}
		else
		{
			//是否需要检测标签
			if (bRequiresTag)
			{
				return (WallActor->ActorHasTag(TagForAllowingWallRun) ? true : false);
			}
		}
	}
	
	//是否需要检测标签 如果需要则不能跑墙
	return !bRequiresTag;
}

bool UTD_WallRunComponent::TopHeightCheck()
{
	//这个功能可以识别靠近玩家胶囊顶部的墙上是否有足够的空间让角色抓住。
	//1. 如果我们还没有开始跑墙，那就防止跑墙
	//2. 如果我们已经开始在墙上跑了，那就防止玩家在墙的顶部的壁架上跑上跑下(这可能发生在墙架向上弯曲的时候，或者如果壁架向下倾斜到墙架的路径上)。

	//我们从简单的直线跟踪开始，因为它最不容易产生误报。
	//轨迹开始于玩家胶囊相对于墙壁顶部的“最大高度”(即，如果轨迹没有击中墙壁，则表明玩家走得太高)，然后向墙壁移动一小段距离。

	// 获取缩放后的胶囊体半高
	float CapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	// 射线起点
	FVector StartPointA = OwnerCharacter->GetActorLocation() + FVector(0.f, 0.f, CapsuleHalfHeight * 0.65f);
	// 射线终点
	FVector EndPointA = StartPointA + AverageWallNormal * CapsuleRadius * -1.7f;

	
	FHitResult HitResult;
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartPointA, EndPointA, TraceTypeQuery, false, TArray<AActor*>(), DrawLineTrace,
	                                          HitResult, true))
	{
		//根据跟踪是否找到合格的墙壁执行不同的操作。
		if (IsWallRunning(HitResult.GetActor()))
		{
			//如果追踪发现玩家处于相对于墙壁的合理高度，则将玩家的z-location记录为最后一个已知的安全高度，并且不再执行进一步的操作。
			LastKnownSafeHeight = OwnerCharacter->GetActorLocation().Z;
			return true;
		}
	}


	//如果行跟踪未能检测到墙壁，我们将执行备份胶囊跟踪。
	//胶囊跟踪的额外尺寸更有可能找到壁的顶部，但这也意味着更有可能发现假阳性，这就是为什么只有在第一次跟踪失败时才会这样做。
	FVector StartPointB = StartPointA;
	FVector EndPointB = StartPointB + AverageWallNormal * -1.1f * CapsuleRadius;
	float Radius = CapsuleRadius * 0.6f;
	float DistanceAboveWall = 0.f;
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartPointB, EndPointB, Radius, TraceTypeQuery, false, TArray<AActor*>(),
	                                            DrawSphereTrace, HitResult, true))
	{
		//根据跟踪是否找到合格的墙壁执行不同的操作。
		if (IsWallRunning(HitResult.GetActor()))
		{
			//如果撞击点低于轨迹的起始位置(表明玩家高于预期位置)，并且玩家当前正在墙上奔跑，
			//那么就降低玩家的高度(他们离墙有多高)，将他们置于当前墙的期望最大高度。
			if (StartPointB.Z > HitResult.ImpactPoint.Z)
			{
				if (bWallRunning)
				{
					DistanceAboveWall = StartPointB.Z - HitResult.ImpactPoint.Z;
					FVector NewLoc = OwnerCharacter->GetActorLocation();
					NewLoc.Z -= DistanceAboveWall;
					OwnerCharacter->SetActorLocation(NewLoc, true);
					LastKnownSafeHeight = NewLoc.Z;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				LastKnownSafeHeight = OwnerCharacter->GetActorLocation().Z;
				return true;
			}
		}
		else
		{
			//如果高度检查失败，但是玩家已经在墙上奔跑，将角色向下移动到最后记录的安全高度
			if (bWallRunning)
			{
				FVector NewLoc = OwnerCharacter->GetActorLocation();
				NewLoc.Z = LastKnownSafeHeight;
				OwnerCharacter->SetActorLocation(NewLoc, true);
			}
			return false;
		}
	}
	else
	{
		if (bWallRunning)
		{
			//如果之前的跟踪没有检测到墙，这意味着角色从前一帧移动了相当大的距离(很可能是由于极低的FPS)，导致我们完全失去了墙。
			//如果发生这种情况，执行故障安全跟踪，试图通过从墙的顶部开始并向下跟踪来找到墙的顶部。
			//除非你的帧率下降到几乎无法玩的水平，否则这将很少被使用。
			FVector StartPointC = StartPointB - AverageWallNormal * CapsuleRadius * 0.5f;
			FVector EndPointC = StartPointC - FVector(0.f, 0.f, CapsuleRadius * 1.5f);
			Radius = CapsuleRadius * 0.6f;
			if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartPointC, EndPointC, Radius, TraceTypeQuery, false, TArray<AActor*>(),
			                                            DrawSphereTrace, HitResult, true))
			{
				//如果故障安全跟踪发现了突出，记录它比期望的高度低多远。
				DistanceAboveWall = StartPointB.Z - HitResult.ImpactPoint.Z;

				FVector NewLoc = OwnerCharacter->GetActorLocation();
				NewLoc.Z -= DistanceAboveWall;
				OwnerCharacter->SetActorLocation(NewLoc, true);
				LastKnownSafeHeight = NewLoc.Z;
				return true;
			}
			else
			{
				//如果高度检查失败，但是玩家已经在墙上奔跑，将角色向下移动到最后记录的安全高度
				if (bWallRunning)
				{
					FVector NewLoc = OwnerCharacter->GetActorLocation();
					NewLoc.Z = LastKnownSafeHeight;
					OwnerCharacter->SetActorLocation(NewLoc, true);
				}
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool UTD_WallRunComponent::BottomHeightCheck(EWallSide Side)
{
	// 获取角色胶囊体的半高
	float CapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	// 计算角色胶囊体（脚底）的位置，这将是射线的起始位置
	FVector StartPointA = OwnerCharacter->GetActorLocation() - FVector(0.f, 0.f, CapsuleHalfHeight);

	// 计算墙的方向
	FVector SideDir = (Side == EWallSide::EWS_Left
						   ? OwnerCharacter->GetActorRightVector() * -1.f
						   : OwnerCharacter->GetActorRightVector());

	// 
	FVector EndPointA = StartPointA + CapsuleRadius * 2.f * SideDir;
	float Radius = CapsuleRadius * 0.6f;
	FHitResult HitResult;

	//检查角色脚附近是否有一堵有效的墙。轨迹从胶囊底部附近开始，然后向Characater的期望一侧移动一小段距离。
	//轨迹长度和半径基于角色的胶囊大小，以支持不同大小的角色。这样做是为了防止角色在身体下半部分挂在墙上时继续向墙上跑。
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartPointA, EndPointA, Radius, TraceTypeQuery, false, TArray<AActor*>(), DrawSphereTrace,
												HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.f))
	{
		//如果检测到有效的墙壁，则返回True，并且不采取进一步的操作。
		return true;
	}
	
	// 没有检测到任何东西，如果处在跑墙状态下，则立即中断跑墙
	if (bWallRunning)
	{
		StopWallRun(MOVE_Falling, 0.0f);
	}
	return false;
}

void UTD_WallRunComponent::StartDurationCountdown()
{
	//如果为wall run指定了最大持续时间，则在wall run开始时调用。
	//启动一个计时器，等待所需的时间，然后停止墙上运行。
	//然后墙跑会在短时间内被禁用，这样玩家就不能立即开始新的墙跑。

	GetWorld()->GetTimerManager().SetTimer(DurationCountdownTimer, this, &ThisClass::WallRunTimeOut, WallRunDurationLimit, false);

}

void UTD_WallRunComponent::WallRunTimeOut()
{
	StopWallRun(MOVE_Falling, 2.0f);
}

void UTD_WallRunComponent::TemporarilyDisableWalkRunning(float DisableTime)
{
	//暂时禁止墙在指定的时间内运行。

	bWallRunningAvailable = false;
	GetWorld()->GetTimerManager().SetTimer(DisabledWallRunTimer, this, &ThisClass::AllowWallRunning, DisableTime, false);
}

void UTD_WallRunComponent::AllowWallRunning()
{
	bWallRunningAvailable = true;
}

void UTD_WallRunComponent::StartMonitoringSpeed()
{
	//启动一个计时器，在墙壁运行期间以设置的间隔监视角色的X和Y速度。
	//这是一种故障保险，用于检测玩家是否在跑墙过程中被卡住，如果他们连续两次被卡住，就会取消跑墙。
	//SpeedTooLow布尔值在监视开始时设置为false，如果测试失败则设置为true。

	bSpeedTooLow = false;

	GetWorld()->GetTimerManager().SetTimer(SpeedMonitorTimer, this, &ThisClass::MonitorSpeed, 0.2f, true);
}

void UTD_WallRunComponent::MonitorSpeed()
{
	//检查角色的X/Y速度是否至少是所需运行速度的25%。如果低于这一数值，就说明墙体被什么东西挡住了。
	//第一次发生这种情况时，我们只将SpeedTooLow设置为true，以标记它已经失败了一次测试——但我们还不想对此做任何事情，以防它只是速度的短暂下降。
	//如果在下一个间隔中测试再次失败，那么我们将停止wall run。
	//如果测试没有连续两次失败，我们将SpeedTooLow设置为false，以便流程可以重新开始。
	//该门只是为了防止我们在SpeedTooLow已经为false时将其重置为false。

	FVector Velocity = OwnerCharacter->GetVelocity();
	FVector Temp = FVector(Velocity.X, Velocity.Y, 0.f);
	if (Temp.Size() < WallRunSpeed * 0.25f)
	{
		if (bSpeedTooLow)
		{
			StopWallRun(MOVE_Falling, 1.0f);
		}
		else
		{
			bSpeedTooLow = true;
			bGateBool_MonitorSpeed = true;
		}
	}
	else
	{
		if (bGateBool_MonitorSpeed)
		{
			bSpeedTooLow = false;
			bGateBool_MonitorSpeed = false;
		}
	}
}

void UTD_WallRunComponent::StopMonitoringSpeed()
{
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(GetWorld(), SpeedMonitorTimer);
}

void UTD_WallRunComponent::DelayedAirControl()
{
	// 在短暂的延迟后，应用必要的移动设置为跳墙。
	// 这种延迟让玩家能够在覆盖跳跃方向之前确定跳跃方向和速度，确保与初始跳跃方向/速度保持一致，而不管玩家输入什么。
	// 加速度与 WallJumpAirControl 参数一起被放大，因为在跳跃过程中，单独设置AirControl并不能提供足够的控制。
	GetWorld()->GetTimerManager().SetTimer(SpeedMonitorTimer, this, &ThisClass::EnableWallJumpAirControl, 0.35f, false);
}

void UTD_WallRunComponent::EnableWallJumpAirControl()
{
	if (OwnerMovementComponent.IsValid())
	{
		OwnerMovementComponent->AirControl = WallJumpAirControl;
		OwnerMovementComponent->MaxAcceleration = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f), FVector2D(2048.f, 6000.f), WallJumpAirControl);
	}
}

void UTD_WallRunComponent::PrintLog(const FString& InStr, const FColor& InColor, float InTime)
{
	if (bDebugScreenInfo)
	{
		UKismetSystemLibrary::PrintString(this, InStr, true, true, InColor, InTime);
	}
}
