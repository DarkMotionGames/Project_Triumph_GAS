// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "MainAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API UMainAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
