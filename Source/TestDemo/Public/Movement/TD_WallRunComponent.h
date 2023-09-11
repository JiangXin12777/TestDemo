#pragma once

#include "CoreMinimal.h"
#include "TD_MovementType.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TD_WallRunComponent.generated.h"

class UCharacterMovementComponent;
class ACharacter;

UCLASS()
class UTD_WallRunComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run", meta=(AllowPrivateAccess))
	TWeakObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement|Wall Run", meta=(AllowPrivateAccess))
	TWeakObjectPtr<UCharacterMovementComponent> OwnerMovementComponent;

public:
	UTD_WallRunComponent();

	/** 当墙跑时进行跳跃 */
	UFUNCTION(BlueprintCallable, Category = "Custom Movement|Wall Run")
	bool StartWallJump();
	
	/** 停止跳跃 */
	UFUNCTION(BlueprintCallable, Category = "Custom Movement|Wall Run")
	void StopWallJump();

protected:
	// ~ Begin UActorComponent Interface
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// ~ End UActorComponent Interface

	/** 开启墙跑 */
	bool StartWallRun(EWallSide Side);
	/** 墙跑 tick */
	void WallRunTick();
	/** 结束墙跑 */
	void StopWallRun(EMovementMode NewMovementMode, float TemporarilyDisableWallRunSeconds);
	/** 墙跑跳跃得 tick 调用 */
	void WallJumpTick();
	/** 将角色靠近墙面 */
	void HugWall();

	
	void Update();

	//更新动画值
	void UpdateAnimationValues();

	/** 初始化角色信息 */
	void InitCharacterInformation();

	/** 管理Tick */
	UFUNCTION()
	void OnMovementModeChangedEvent(class ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);

	/** 检查地板 */
	UFUNCTION()
	void OnActorHitEvent_CheckFloor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	/** 检测墙 */
	void DetectWall(EWallSide Side);

	//相对于Capsule 和 Wall的射线
	UFUNCTION(BlueprintCallable, Category = "Movement|Trace")
	bool LineTraceRelativeToCapsuleAndWall(FVector CurrentWallNormal, FVector InCurrentWallLocation, float InitalTraceLength,
										   float StartOffset_Backwards, float StartOffset_AwayFromWall, float EndOffset_Backwards,
										   float EndOffset_AwayFromWall, EWallSide Side, FHitResult& HitResult);

	//检查角落角度
	bool CheckCornerAngle(FVector FirstWallNormal, FVector SecondWallNormal);
	
	/**
	 * 判断传入的 Actor 是否支持墙跑功能，通过 ActorTag 检测。
	 *
	 * @param WallActor 需要检测的 Actor
	 * @return 返回是否可以在当前物体上进行墙跑
	 */
	bool IsWallRunning(AActor* WallActor);
	
	/** 顶部高度检查 */
	bool TopHeightCheck();
	
	/** 底部高度检查 */
	bool BottomHeightCheck(EWallSide Side);

	/** 开始持续时间倒计时 */
	void StartDurationCountdown();

	/** 墙跑超时 */
	void WallRunTimeOut();

	/** 暂时禁用步行跑步 */
	void TemporarilyDisableWalkRunning(float DisableTime);
	/** 允许墙跑 */
	void AllowWallRunning();

	//开始监控速度
	void StartMonitoringSpeed();

	//监控速度
	bool bGateBool_MonitorSpeed = false;
	void MonitorSpeed();

	//停止监控速度
	void StopMonitoringSpeed();

	/** 延迟 AirControl */
	void DelayedAirControl();
	/** 启用墙壁跳跃 AirControl */
	void EnableWallJumpAirControl();


	/** 打印 debug 信息 */
	void PrintLog(const FString& InStr, const FColor& InColor, float InTime);

public:
	// ~Begin Wall Run Parameters
	/** 跑墙速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Parameters")
	float WallRunSpeed = 800.f;
	
	/** 弧度系数 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Parameters")
	float ArcAmount = 0.8f;
	
	/** 跑墙持续时间限制 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Parameters")
	float WallRunDurationLimit = 3.f;
	
	/** 外角角度限制 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Parameters")
	float OutsideCornerAngleLimit = 45.f;

	/** 内角角度限制 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Parameters")
	float InsideCornerAngleLimit = 45.f;

	/** 跳墙角度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Parameters")
	float WallJumpAngle = 45.f;
	
	/** 跳墙水平速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Parameters")
	float WallJumpHorizontalVelocity = 800.f;

	/** 跳墙垂直速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Parameters")
	float WallJumpVerticalVelocity = 500.f;

	/** 跳墙空中控制系数 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Parameters")
	float WallJumpAirControl = 0.3f;

	/** 是否请求Tag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Parameters")
	bool bRequiresTag = false;
	
	/** 允许跑墙的标签 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Parameters")
	FName TagForAllowingWallRun = TEXT("WallRun");

	/** 防止跑墙的标签 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Parameters")
	FName TagForPreventingWallRun = TEXT("NoWallRun");
	// ~End Wall Run Parameters

	
	//~ Begin WallRunMovementSettings
	/** 跑墙加速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Movement Settings")
	float WallRunAcceleration = 50000.f;
	
	/** 跑墙制动减速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Wall Run Movement Settings")
	float WallRunBrakingDeceleration = 500.f;
	//~ End WallRunMovementSettings  

	
	// ~Begin Current Wall Information
	/** 墙壁侧面 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Current Wall Information")
	EWallSide WallSide = EWallSide::EWS_Right;
	
	/** 当前墙的位置 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Current Wall Information")
	FVector CurrentWallLocation;

	/** 主要射线法线 */
	UPROPERTY(BlueprintReadOnly, Category="Custom Movement|Current Wall Information")
	FVector PrimaryTraceNormal;
	
	/** 次要射线法线 */
	UPROPERTY(BlueprintReadOnly, Category="Custom Movement|Current Wall Information")
	FVector SecondaryTraceNormal;

	/** 平均墙壁法线 */
	UPROPERTY(BlueprintReadOnly, Category="Custom Movement|Current Wall Information")
	FVector AverageWallNormal;
	
	/** 上一帧的速度 */
	UPROPERTY(BlueprintReadOnly, Category="Custom Movement|Current Wall Information")
	FVector PreviousVelocity;

	/** 当前拐角角度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Current Wall Information")
	float CurrentCornerAngle;

	/** 最后已知的安全高度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Current Wall Information")
	float LastKnownSafeHeight;
	
	/** 当前墙面运行持续时间 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Current Wall Information")
	float CurrentWallRunDuration;

	/** 与墙壁的距离 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Current Wall Information")
	float DistanceFromWall;
	// ~End Current Wall Information

	
	// ~Begin States
	/** 是否允许跑墙 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|States")
	bool bWallRunningAvailable = true;
	
	/** 跑墙中 */
	UPROPERTY(BlueprintReadOnly, Category="Custom Movement|States")
	bool bWallRunning = false;
	
	/** 跳墙中 */
	UPROPERTY(BlueprintReadOnly, Category="Custom Movement|States")
	bool bWallJumping = false;
	
	/** 是否速度太低 */
	UPROPERTY(BlueprintReadOnly, Category="Custom Movement|States")
	bool bSpeedTooLow = false;
	// ~End States

	/** 当前弧度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Values")
	float CurrentArcAngle;

	/** 当前转向速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Values")
	float CurrentTurnRate;

	/** 上一帧的旋转 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Values")
	FRotator PreviousRotation;
	
protected:
	/** 跟踪类型查询 */
	UPROPERTY(EditAnywhere, Category = "Custom Movement|Trace")
	TEnumAsByte<ETraceTypeQuery> TraceTypeQuery;
	
	// ~Begin Debug
	/** 绘制调试跟踪（线条） */
	UPROPERTY(EditAnywhere, Category = "Custom Movement|Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawLineTrace = EDrawDebugTrace::None;

	/** 绘制调试跟踪(球型) */
	UPROPERTY(EditAnywhere, Category = "Custom Movement|Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawSphereTrace = EDrawDebugTrace::None;
	
	/** 是否展示 debug 信息 */
	UPROPERTY(EditAnywhere, Category = "Custom Movement|Debug")
	bool bDebugScreenInfo = false;

	/** 是否显示Debug线条 */
	UPROPERTY(EditAnywhere, Category = "Custom Movement|Debug")
	bool bShowTraceLine = false;
	// ~End Debug

	// ~Begin Character default info
	/** 胶囊半径 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Character Information")
	float CapsuleRadius;
	
	/** 默认最大飞行速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Character Information")
	float DefaultMaxFlySpeed;
	
	/** 默认原点旋转 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Character Information")
	bool bDefaultOrientRotation;
	
	/** 默认最大加速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Character Information")
	float DefaultMaxAcceleration;
	
	/** 默认飞行 制动 减速度  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Character Information")
	float DefaultBrakingDecelerationFlying;
	
	/** 默认空中控制系数 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Character Information")
	float DefaultAirControl;
	// ~End Character default info
	

	/** 主要射线长度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Custom Movement|Trace properties")
	float PrimaryTraceSize;
	
	
	// ~Begin Timer Handle
	/** 跑墙持续时间计时器 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Timer Handle")
	FTimerHandle DurationCountdownTimer;

	/** 跑墙禁用计时器 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Timer Handle")
	FTimerHandle DisabledWallRunTimer;

	/** 跑墙速度监控计时器 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Timer Handle")
	FTimerHandle SpeedMonitorTimer;

	/** 跳墙空中控制计时器 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Timer Handle")
	FTimerHandle WallJumpAirControlTimer;
	// ~End Timer Handle
};