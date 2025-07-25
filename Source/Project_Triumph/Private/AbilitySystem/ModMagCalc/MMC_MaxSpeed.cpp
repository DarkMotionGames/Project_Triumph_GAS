// © 2024 DMG. All rights reserved.


#include "AbilitySystem/ModMagCalc/MMC_MaxSpeed.h"
#include "AbilitySystem/AttributeSetBase.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxSpeed::UMMC_MaxSpeed()
{
	AgilityMagnitudeCapture.AttributeToCapture = UAttributeSetBase::GetAgilityAttribute();
	AgilityMagnitudeCapture.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AgilityMagnitudeCapture.bSnapshot = false;
    
	RelevantAttributesToCapture.Add(AgilityMagnitudeCapture);
}

float UMMC_MaxSpeed::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTag;
	EvaluationParameters.TargetTags = TargetTag;

	float Agility = 0.f;
	GetCapturedAttributeMagnitude(AgilityMagnitudeCapture, Spec, EvaluationParameters, Agility);
	Agility = FMath::Max<float>(Agility, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 10.f + 2.5f * Agility + 10.f * PlayerLevel;
}