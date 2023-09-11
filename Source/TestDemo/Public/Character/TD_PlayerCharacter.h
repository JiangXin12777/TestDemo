#pragma once

#include "CoreMinimal.h"
#include "GameMode/TD_CharacterBase.h"
#include "TD_PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNinjaInputManagerComponent;

/**
 * 玩家基类
 */
UCLASS(Config = Game)
class ATD_PlayerCharacter
	: public ATD_CharacterBase
{
	GENERATED_BODY()

	/** !吊臂组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** !相机组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** !输入组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNinjaInputManagerComponent> InputManagerComponent;
	
public:
	ATD_PlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 获取吊臂组件 */
	FORCEINLINE USpringArmComponent* GetCameraBoom() const;
	/** 获取相机组件 */
	FORCEINLINE UCameraComponent* GetFollowCamera() const;
	/** 获取输入组件 */
	FORCEINLINE UNinjaInputManagerComponent* GetInputManagerComponent() const;

protected:
	virtual void BeginPlay() override;

	/** Called for movement input */
	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& Value);
};