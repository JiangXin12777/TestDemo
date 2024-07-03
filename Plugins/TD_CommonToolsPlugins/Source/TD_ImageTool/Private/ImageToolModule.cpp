#include "ImageToolModule.h"
#include "ImageToolBPLibrary.h"

#define LOCTEXT_NAMESPACE "ImageTool"

void FImageToolModule::StartupModule()
{
}

void FImageToolModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FImageToolModule, ImageTool)