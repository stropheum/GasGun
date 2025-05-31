#pragma once

#include "CoreMinimal.h"
#include "Runtime/GameplayTags/Public/NativeGameplayTags.h"


namespace NativeGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_NoLevel);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Type_Cooldown);

	namespace Projectile
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DamageType_DirectDamage);
	}
}
