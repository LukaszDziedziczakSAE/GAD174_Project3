// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAD174_Project3 : ModuleRules
{
	public GAD174_Project3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
