#include "MMC_MaxMana.h"

#include "Aura/Interact/CombatInterface.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntellDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntellDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntellDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntellDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intell = 0.f;
	GetCapturedAttributeMagnitude(IntellDef, Spec, EvaluationParameters, Intell);
	Intell = FMath::Max<float>(Intell, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	// return pos + (coff * (Intell + Pre)) + BaseValueEachLevel * PlayerLevel.
	return 50.f + 2.5f * Intell + 15.f * PlayerLevel;
}
