// Copyright (C) 2023 Shpz<shaaaaapz@gmail.com>. All Rights Reserved.

#pragma once

#include "GTMGInterface.h"

#define GTMG_CPP

/**
 *	GameplayTag macro C++ 代码生成器
 *	
 *	GameplayTag macro C++ code generator
 */
class FGTMGCpp : public IGTMGInterface
{
protected:
	virtual FString CombineGameplayTagMacroCode() override;

	/**
	 *	计算对齐数量
	 *	
	 *	Calculate alignment Tab Num
	 */
	int32 CalculateTabNum();

	/**
	 *	拼接注释
	 *	
	 *	Combine Comment
	 */
	void CombineComment(FString& Code, const FString& DevComment);

	/**
	 *	拼接对齐
	 *	
	 *	Combine Alignment
	 */
	void CombineAlignment(FString& Code, FString& Macro, int32 MaxCharNum);
};
