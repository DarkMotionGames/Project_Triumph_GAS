// © 2024 DMG. All rights reserved.


#include "AbilitySystem/MainAbilitySystemGlobals.h"

#include "MainAbilitySystemTypes.h"

FGameplayEffectContext* UMainAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FMainGameplayEffectContext();
}
