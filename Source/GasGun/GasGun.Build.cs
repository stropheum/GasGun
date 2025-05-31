// Copyright 2025 Dale "Stropheum" Diaz

using UnrealBuildTool;

public class GasGun : ModuleRules
{
	public GasGun(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"AIModule",
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"GameplayAbilities", 
			"GameplayTasks", 
			"GameplayTags",
			"NavigationSystem",
			"Niagara",
			"NetCommon",
			"Networking",
		});
	}
}