// Copyright (C) 2023 Shpz<shaaaaapz@gmail.com>. All Rights Reserved.

#pragma once

#include "GTMGDefinition.h"
#include "GameplayTagsManager.h"
#include "GameplayTagsSettings.h"
#include "GTMGSetting.h"
#include "Misc/FileHelper.h"

/**
 *	代码生成器接口，所有语言的代码生成器都应该实现这个接口
 *	Code generator interface, all code generator of language should implement this.
 */
class IGTMGInterface
{
public:
	IGTMGInterface()
	{
	}

	virtual ~IGTMGInterface()
	{
	}

	/**
	 *	开始生成 GameplayTag 宏代码
	 *	
	 *	Start generate GameplayTag macro code.
	 */
	void Generate()
	{
		LoadGameplayTag();
		CollectGameplayTag();
		SaveGameplayTag();
		CleanCache();
	}

protected:
	// 无重复的 GameplayTag
	// No duplicate gameplay tags
	TArray<FName> NoDuplicateGameplayTags;

	// 无重复的 GameplayTagTableRow
	// No duplicate GameplayTagTableRow
	TMap<FName, const FGameplayTagTableRow* const> NoDuplicateGameplayTagTableRow;

	// GameplayTagTableList 里引用的 DataTable 资产	
	// DataTable assets referenced in GameplayTagTableList
	TArray<const UDataTable*> DataTables;

protected:
	/**
	 *	拼接 GameplayTag 宏代码，子代码生成器必须重写
	 *	
	 *	Combine GameplayTag macro code, sub code generator must implement.
	 */
	virtual FString CombineGameplayTagMacroCode() = 0;

	/**
	 *	加载 GameplayTag 数据表资产
	 *	
	 *	Load GameplayTag CompositeDataTable asset
	 */
	virtual void LoadGameplayTag()
	{
		TArray<FSoftObjectPath> List = GetDefault<UGameplayTagsSettings>()->GameplayTagTableList;
		DataTables.Empty(List.Num());

		for (const FSoftObjectPath& Path : List)
		{
			DataTables.Add(LoadObject<UDataTable>(nullptr, *Path.ToString()));
		}
	}

	/**
	 *	收集 GameplayTag，并警告重复
	 *	
	 *	Collect GameplayTag, and warning duplicate
	 */
	virtual void CollectGameplayTag()
	{
		CollectGameplayTagFromGameplayTagTableList();
		CollectGameplayTagFromGameplayTagList();
	}

	/**
	 *	收集 GameplayTagTableList 里的 GameplayTag
	 *	
	 *	Collect GameplayTag from GameplayTagTableList
	 */
	virtual void CollectGameplayTagFromGameplayTagTableList()
	{
		FString Context = "Collect GameplayTag from DataTables";

		for (const UDataTable* DataTable : DataTables)
		{
			TArray<FName> RowNames = DataTable->GetRowNames();
			for (const FName& RowName : RowNames)
			{
				FString DataTableName = DataTable->GetName();
				const FGameplayTagTableRow* TableRow = DataTable->FindRow<FGameplayTagTableRow>(RowName, Context);

				CheckGameplayTagTableRowNameDuplicate(DataTableName, RowName, TableRow);
				
				HandleGameplayTagDuplicate(DataTableName, RowName, TableRow);
			}
		}
	}

	/**
	 *	收集 GameplayTagList 里的 GameplayTag
	 *	
	 *	Collect GameplayTag from GameplayTagList
	 */
	virtual void CollectGameplayTagFromGameplayTagList()
	{
		FString Context = "Collect GameplayTag from DataTables";
		const TArray<FGameplayTagTableRow>& List = GetDefault<UGameplayTagsSettings>()->GameplayTagList;

		for (const FGameplayTagTableRow& TableRow : List)
		{
			HandleGameplayTagDuplicate(TableRow);
		}
	}

	/**
	 *	检查 RowName 是否重复。当使用 GameplayTagCompositeDataTable 时，RowName 有可能出现重复
	 *	
	 *	Check if RowName is duplicate. When using GameplayTagCompositeDataTable, RowName may have duplicates
	 */
	virtual void CheckGameplayTagTableRowNameDuplicate(const FString& DataTableName,
	                                                    const FName& RowName,
	                                                    const FGameplayTagTableRow* TableRow)
	{
		if (NoDuplicateGameplayTagTableRow.Find(RowName) != nullptr)
		{
			UE_LOG(GTMG_WARNING,
			       Warning,
			       TEXT("DataTable \"%s\", RowName \"%s\" duplicate!"),
			       *DataTableName,
			       *RowName.ToString());
		}
	}

	/**
	 *	判定 GameplayTag 是否重复
	 *	
	 *	Judge if RowName is duplicate
	 */
	virtual void HandleGameplayTagDuplicate(const FString& DataTableName,
	                                        const FName& RowName,
	                                        const FGameplayTagTableRow* TableRow)
	{
		const FName& TagName = TableRow->Tag;
		if (NoDuplicateGameplayTags.Find(TagName) == INDEX_NONE)
		{
			NoDuplicateGameplayTags.Add(TagName);
			NoDuplicateGameplayTagTableRow.Add(RowName, TableRow);
		}
		else
		{
			UE_LOG(GTMG_WARNING,
			       Warning,
			       TEXT("GameplayTagTableList DataTable \"%s\", RowName \"%s\", GameplayTag \"%s\" duplicate! "),
			       *DataTableName,
			       *RowName.ToString(),
			       *TagName.ToString());
		}
	}

	/**
	 *	判定 GameplayTag 是否重复
	 *	
	 *	Judge GameplayTag is replicate
	 */
	virtual void HandleGameplayTagDuplicate(const FGameplayTagTableRow& TableRow)
	{
		FName TagName = TableRow.Tag;
		if (NoDuplicateGameplayTags.Find(TagName) == INDEX_NONE)
		{
			NoDuplicateGameplayTags.Add(TagName);

			FString TagString = TagName.ToString();
			const FString ReplaceFrom = ".";
			const FString ReplaceTo = "_";
			
			FString RowNameString = TagString.Replace(*ReplaceFrom, *ReplaceTo);
			FName RowName = FName(*RowNameString);
			NoDuplicateGameplayTagTableRow.Add(RowName, &TableRow);
		}
		else
		{
			UE_LOG(GTMG_WARNING,
			       Warning,
			       TEXT("GameplayTagList GameplayTag \"%s\" duplicate! "),
			       *TagName.ToString());
		}
	}

	/**
	 *	保存 GameplayTag 宏代码
	 *	
	 *	Save GameplayTag macro code to file.
	 */
	void SaveGameplayTag()
	{
		const FString Code = CombineGameplayTagMacroCode();

		FString OutputDirectory = GetDefault<UGTMGSetting>()->CppMacroFileOutputPath.Path;
		FString OutputName = GetDefault<UGTMGSetting>()->CppMacroFileOutputName;
		OutputName.Append(".h");

		const FString OutputPath = FPaths::Combine(OutputDirectory, OutputName);
		bool bResult = FFileHelper::SaveStringToFile(Code, *OutputPath, FFileHelper::EEncodingOptions::ForceUTF8);
		if (bResult)
		{
			UE_LOG(GTMG_LOG, Log, TEXT("Save macro file successfully."));
		}
		else
		{
			UE_LOG(GTMG_WARNING, Warning, TEXT("Save macro file failed."));
		}
	}

	/**
	 *	清理缓存
	 *
	 *	Clean Cache
	 */
	void CleanCache()
	{
		NoDuplicateGameplayTagTableRow.Empty(0);
		NoDuplicateGameplayTags.Empty(0);
		DataTables.Empty(0);
	}
};
