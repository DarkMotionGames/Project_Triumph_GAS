// © 2024 DMG. All rights reserved.


#include "AbilitySystem/ModMagCalc/MMC_MaxStamina.h"

#include "AbilitySystem/AttributeSetBase.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxStamina::UMMC_MaxStamina()
{
	StrengthMagnitudeCapture.AttributeToCapture = UAttributeSetBase::GetStrengthAttribute();
	StrengthMagnitudeCapture.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrengthMagnitudeCapture.bSnapshot = false;
    
	RelevantAttributesToCapture.Add(StrengthMagnitudeCapture);
}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTag;
	EvaluationParameters.TargetTags = TargetTag;

	float Strength = 0.f;
	GetCapturedAttributeMagnitude(StrengthMagnitudeCapture, Spec, EvaluationParameters, Strength);
	Strength = FMath::Max<float>(Strength, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Strength + 10.f * PlayerLevel;
}
