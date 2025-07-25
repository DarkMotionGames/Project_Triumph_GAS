// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxStamina.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API UMMC_MaxStamina : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UMMC_MaxStamina();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition StrengthMagnitudeCapture;
};
