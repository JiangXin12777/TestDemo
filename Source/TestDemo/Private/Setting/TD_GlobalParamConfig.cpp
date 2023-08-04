#include "Setting/TD_GlobalParamConfig.h"

UTD_GlobalParamConfig::UTD_GlobalParamConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FName UTD_GlobalParamConfig::GetContainerName() const
{
	return Super::GetContainerName();
}

FName UTD_GlobalParamConfig::GetCategoryName() const
{
	static const FName OutCategoryName = TEXT("TD");
	return OutCategoryName;
}

FName UTD_GlobalParamConfig::GetSectionName() const
{
	return Super::GetSectionName();
}
