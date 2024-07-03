// Copyright Epic Games, Inc. All Rights Reserved.
using EpicGames.Core;
using Microsoft.Extensions.Logging;
using System.IO;
using UnrealBuildTool;

public class ImageTool : ModuleRules
{
	public ImageTool(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
		);
	
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
            }
		);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
                "CoreUObject",
				"Engine",
                "Slate",
				"SlateCore",
				"UMG",
				"RHI",
                "HTTP",
				"RenderCore"
            }
        );

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            var baseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "../.."));
            var libDirectory = Path.Combine(baseDirectory, "ThirdParty", "webp", Target.Platform.ToString());

            // Add the import library
            PublicIncludePaths.Add(Path.Combine(libDirectory, "include"));
            PublicAdditionalLibraries.Add(Path.Combine(libDirectory, "lib", "libwebp.lib"));
        }
    }
}
