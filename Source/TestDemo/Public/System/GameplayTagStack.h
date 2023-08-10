// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "GameplayTagStack.generated.h"

struct FGameplayTagStackContainer;
struct FNetDeltaSerializeInfo;

/**
 * 表示一叠游戏标签（标签 + 计数）
 */
USTRUCT(BlueprintType)
struct FGameplayTagStack : public FFastArraySerializerItem
{
	GENERATED_BODY()

	friend FGameplayTagStackContainer;
	
	FGameplayTagStack()
	{
	}

	FGameplayTagStack(FGameplayTag InTag, int32 InStackCount)
		: Tag(InTag)
		  , StackCount(InStackCount)
	{
	}

	FString GetDebugString() const;

private:
	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY()
	int32 StackCount = 0;
};


/** 游戏标签堆栈的容器 */
USTRUCT(BlueprintType)
struct FGameplayTagStackContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	FGameplayTagStackContainer()
	//	: Owner(nullptr)
	{
	}

public:
	//将指定数量的堆栈添加到标记（如果 StackCount 低于 1，则不执行任何操作）
	void AddStack(FGameplayTag Tag, int32 StackCount);

	// 从标记中删除指定数量的堆栈（如果 StackCount 低于 1，则不执行任何操作）
	void RemoveStack(FGameplayTag Tag, int32 StackCount);

	// 返回指定标签的堆栈计数（如果标签不存在，则返回 0）
	int32 GetStackCount(FGameplayTag Tag) const
	{
		return TagToCountMap.FindRef(Tag);
	}

	// 如果至少有一个指定标记的堆栈，则返回 true
	bool ContainsTag(FGameplayTag Tag) const
	{
		return TagToCountMap.Contains(Tag);
	}

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FGameplayTagStack, FGameplayTagStackContainer>(Stacks, DeltaParms, *this);
	}

private:
	// 复制的游戏标签堆栈列表
	UPROPERTY()
	TArray<FGameplayTagStack> Stacks;

	// 查询的标签堆栈加速列表
	TMap<FGameplayTag, int32> TagToCountMap;
};

template <>
struct TStructOpsTypeTraits<FGameplayTagStackContainer> : public TStructOpsTypeTraitsBase2<FGameplayTagStackContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
