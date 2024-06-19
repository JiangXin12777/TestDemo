// Copyright Epic Games, Inc. All Rights Reserved.

#include "TD_RegularExpression.h"

#define LOCTEXT_NAMESPACE "FTD_RegularExpressionModule"

void FTD_RegularExpressionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FTD_RegularExpressionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTD_RegularExpressionModule, TD_RegularExpression)