// Pietras

using UnrealBuildTool;
using System.Collections.Generic;

public class ClosedGroundsEditorTarget : TargetRules
{
	public ClosedGroundsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "ClosedGrounds" } );
	}
}
