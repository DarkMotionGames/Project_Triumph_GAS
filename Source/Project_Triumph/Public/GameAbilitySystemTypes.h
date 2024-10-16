// © 2024 DMG. All rights reserved.
#pragma once

#include "GameplayEffectTypes.h"
#include "GameAbilitySystemTypes.generated.h"

USTRUCT(BlueprintType)
struct FMainGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }

	void SetIsBlockedHit(const bool InBlockHit) { bIsBlockedHit = InBlockHit; }
	void SetIsCriticalHit(const bool InCriticalHit) { bIsCriticalHit = InCriticalHit; }
	
	/** Returns the actual struct used for serialization */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayEffectContext::StaticStruct();
	}
	
	/** Custom serialization */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
	
};
