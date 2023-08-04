// Copyright (C) 2023 Shpz<shaaaaapz@gmail.com>. All Rights Reserved.

#include "GTMGCpp.h"
#include "Misc/FileHelper.h"

FString FGTMGCpp::CombineGameplayTagMacroCode()
{
	FString Code = "#pragma once\n\n";
	int32 MaxCharNum = CalculateTabNum();

	FString Context = "Write Macro File";
	for (const TTuple<FName, const FGameplayTagTableRow*> Tuple : NoDuplicateGameplayTagTableRow)
	{
		FString TagString = Tuple.Value->Tag.ToString();
		FString DevComment = Tuple.Value->DevComment;
		const FName& RowName = Tuple.Key;

		FString MacroString = RowName.ToString();

		Code.Append("#ifndef ");
		Code.Append(MacroString);
		Code.Append("\n");

		CombineComment(Code, DevComment);
		CombineAlignment(Code, MacroString, MaxCharNum);

		Code.Append("FGameplayTag::RequestGameplayTag(FName(\"");
		Code.Append(TagString);
		Code.Append("\"))\n");
		Code.Append("#endif\n\n");
	}

	return Code;
}

int32 FGTMGCpp::CalculateTabNum()
{
	int32 MaxCharNum = 40;
	for (const TTuple<FName, const FGameplayTagTableRow*> Tuple : NoDuplicateGameplayTagTableRow)
	{
		int32 CharNum = GetNum(Tuple.Key.ToString());
		if (CharNum > MaxCharNum)
		{
			MaxCharNum = CharNum;
		}
	}
	
	return (MaxCharNum / 4 + 1) * 4;
}

void FGTMGCpp::CombineComment(FString& Code, const FString& DevComment)
{
	ECommentStyle Style = GetDefault<UGTMGSetting>()->CommentStyle;
	if (Style == ECommentStyle::Line)
	{
		Code.Append("// ");
		Code.Append(DevComment);
		Code.Append("\n");
	}
	else if (Style == ECommentStyle::Documentation)
	{
		Code.Append("/**\n");
		Code.Append(" *	");
		Code.Append(DevComment);
		Code.Append("\n");
		Code.Append(" */\n");
	}
}

void FGTMGCpp::CombineAlignment(FString& Code, FString& Macro, int32 MaxCharNum)
{
	Code.Append("#define ");
	Code.Append(Macro);

	int32 CharNum = GetNum(Macro);
	ECommentStyle Style = GetDefault<UGTMGSetting>()->CommentStyle;

	int32 TabNum = MaxCharNum - CharNum;
	if (Style == ECommentStyle::Line)
	{
		if (TabNum % 4 == 0)
		{
			TabNum = TabNum / 4 + 1;
		}
		else
		{
			TabNum = TabNum / 4 + 2;
		}
	}
	
	for (int32 i = 0; i < TabNum; i++)
	{
		Code.Append("\t");
	}
}
