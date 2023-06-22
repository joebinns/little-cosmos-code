// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameProject3_Team02 : ModuleRules
{
	public GameProject3_Team02(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput","Slate", "SlateCore", "UMG" });
	}
}
