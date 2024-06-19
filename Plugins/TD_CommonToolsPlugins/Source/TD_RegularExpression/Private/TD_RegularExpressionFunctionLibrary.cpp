// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_RegularExpressionFunctionLibrary.h"

bool UTD_RegularExpressionFunctionLibrary::RegexMatch(const FString& Str, const FString& Pattern,
	TArray<FString>& Result)
{
	FRegexPattern MatherPatter(Pattern);
	FRegexMatcher Matcher(MatherPatter, Str);

	while (Matcher.FindNext())
	{
		Result.Add(Matcher.GetCaptureGroup(0));

	}

	return Result.Num() == 0 ? false : true;
}

bool UTD_RegularExpressionFunctionLibrary::IsStrongPassword(const FString& InStr)
{
	// 正则表达式模式
	FString Pattern = TEXT(
		"(?=.*[a-z])"       // 包含小写字母
		"(?=.*[A-Z])"       // 包含大写字母
		"(?=.*\\d)"         // 包含数字
		"(?=.*[^a-zA-Z\\d])" // 包含特殊字符
		".{8,16}"           // 密码长度为8-16位
	);

	FRegexPattern RegexPattern(Pattern);
	FRegexMatcher RegexMatcher(RegexPattern, InStr);
	return RegexMatcher.FindNext();
}

bool UTD_RegularExpressionFunctionLibrary::IsPhoneNumber(const FString& Text)
{
	// 正则表达式模式：匹配手机号码
	FString Pattern = TEXT("^((13[0-9])|(15[^4,\\D])|(17[0-9])|(18[0-9]))\\d{8}$");

	FRegexPattern RegexPattern(Pattern);
	FRegexMatcher RegexMatcher(RegexPattern, Text);

	return RegexMatcher.FindNext();
}

bool UTD_RegularExpressionFunctionLibrary::IsEmailAddress(const FString& Text)
{
	// 正则表达式模式：匹配电子邮件地址
	FString Pattern = TEXT("^([a-z0-9A-Z]+[-|\\.]?)+[a-z0-9A-Z]@([a-z0-9A-Z]+(-[a-z0-9A-Z]+)?\\.)+[a-zA-Z]{2,}$");

	FRegexPattern RegexPattern(Pattern);
	FRegexMatcher RegexMatcher(RegexPattern, Text);

	return RegexMatcher.FindNext();
}

bool UTD_RegularExpressionFunctionLibrary::IsValidURL(const FString& InStr)
{
	FString Pattern = TEXT(R"((http|https)://([\w.-]+)(:[0-9]+)?(/[\w.-]*)*)");

	FRegexPattern RegexPattern(Pattern);
	FRegexMatcher RegexMatcher(RegexPattern, InStr);

	return RegexMatcher.FindNext();
}
