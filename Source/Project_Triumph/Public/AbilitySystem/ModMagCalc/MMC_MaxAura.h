// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxAura.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API UMMC_MaxAura : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UMMC_MaxAura();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceMagnitudeCapture;
};
