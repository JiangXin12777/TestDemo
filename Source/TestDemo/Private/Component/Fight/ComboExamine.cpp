#include "Component/Fight/ComboExamine.h"

FComboExamine::FComboExamine()
	: ComboIndex(INDEX_NONE)
	, bLongPress(false)
	, bShortPress(false)
	, MaxIndex(4)
	, ComboClass(nullptr)
{
}

void FComboExamine::UpdateComboIndex()
{
	check(MaxIndex > 0);

	ComboIndex++;
	if (ComboIndex > MaxIndex)
	{
		ComboIndex = 1;
	}
}

void FComboExamine::Press()
{
	if (ComboIndex == INDEX_NONE)
	{
		ComboIndex++;
	}

	bShortPress = true;
	bLongPress = true;
}

void FComboExamine::Released()
{
	bLongPress = false;
}

void FComboExamine::Reset()
{
	*this = FComboExamine();
}
