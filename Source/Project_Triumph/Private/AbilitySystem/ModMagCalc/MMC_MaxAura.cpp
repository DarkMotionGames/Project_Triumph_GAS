// © 2024 DMG. All rights reserved.


#include "AbilitySystem/ModMagCalc/MMC_MaxAura.h"

#include "AbilitySystem/AttributeSetBase.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxAura::UMMC_MaxAura()
{
	IntelligenceMagnitudeCapture.AttributeToCapture = UAttributeSetBase::GetIntelligenceAttribute();
	IntelligenceMagnitudeCapture.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceMagnitudeCapture.bSnapshot = false;
    
	RelevantAttributesToCapture.Add(IntelligenceMagnitudeCapture);
}

float UMMC_MaxAura::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTag;
	EvaluationParameters.TargetTags = TargetTag;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceMagnitudeCapture, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 50.f + 2.5f * Intelligence + 5.f * PlayerLevel;
}
