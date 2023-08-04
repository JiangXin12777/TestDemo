#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "TD_AttributeSet.generated.h"

/**
 * 封装属性注册的宏
 * 本质上使用的是 AttributeSet.h 中的属性宏，这里为了减少代码量 进行二次封装
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 角色属性集基类
 */
UCLASS()
class UTD_AttributeSet
	: public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UTD_AttributeSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};