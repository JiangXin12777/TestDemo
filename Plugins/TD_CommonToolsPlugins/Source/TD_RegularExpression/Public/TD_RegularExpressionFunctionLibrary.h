// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TD_RegularExpressionFunctionLibrary.generated.h"

/**
 * Need help with regex? Try https://regex101.com/
 */
UCLASS()
class TD_REGULAREXPRESSION_API UTD_RegularExpressionFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Find Str with RegularExpression", Keywords = "RegularExpression sample test testing"), Category = "RegularExpression")
	static bool RegexMatch(const FString& Str, const FString& Pattern, TArray<FString>& Result);
	
	/** 大小写字母，数字，特殊字符，长度8-16 */
	UFUNCTION(BlueprintPure, Category = "RegularExpression")
	static bool IsStrongPassword(const FString& InStr);

	/** 校验手机号格式 */
	UFUNCTION(BlueprintPure, Category = "RegularExpression")
	static bool IsPhoneNumber(const FString& Text);

	/** 校验邮箱格式 */
	UFUNCTION(BlueprintPure, Category = "RegularExpression")
	static bool IsEmailAddress(const FString& Text);

	/** 是否为有效URL */
	UFUNCTION(BlueprintPure, Category = "RegularExpression")
	static bool IsValidURL(const FString& InStr);
};
