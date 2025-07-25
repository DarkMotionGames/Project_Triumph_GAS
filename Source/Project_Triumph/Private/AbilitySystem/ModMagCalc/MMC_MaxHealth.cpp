// © 2024 DMG. All rights reserved.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AttributeSetBase.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorMagnitudeCapture.AttributeToCapture = UAttributeSetBase::GetVigorAttribute();
	VigorMagnitudeCapture.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorMagnitudeCapture.bSnapshot = false;
    
	RelevantAttributesToCapture.Add(VigorMagnitudeCapture);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTag;
	EvaluationParameters.TargetTags = TargetTag;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorMagnitudeCapture, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
