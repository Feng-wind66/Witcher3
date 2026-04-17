// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ExecCalc/PhysicalDamage.h"
#include"GAS/AS.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PunctureResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ChopResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LuntImpactResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BreakingDefenseValue);
	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAS, Health, Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAS, CritRate,Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAS, CritDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAS, PunctureResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAS, ChopResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAS, LuntImpactResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAS, BreakingDefenseValue, Target, false);
	}
};
static const FDamageStatics& DamageStaticsData()
{
	static FDamageStatics DamageStatics;
	return DamageStatics;
}
UPhysicalDamage::UPhysicalDamage()
{
	RelevantAttributesToCapture.Add(FDamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(FDamageStatics().CritRateDef);
	RelevantAttributesToCapture.Add(FDamageStatics().CritDamageDef);
	RelevantAttributesToCapture.Add(FDamageStatics().PunctureResistanceDef);
	RelevantAttributesToCapture.Add(FDamageStatics().ChopResistanceDef);
	RelevantAttributesToCapture.Add(FDamageStatics().LuntImpactResistanceDef);
	RelevantAttributesToCapture.Add(FDamageStatics().BreakingDefenseValueDef);
}

void UPhysicalDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent*OwnerASC=ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!IsValid(OwnerASC) || !IsValid(TargetASC))return;
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	float Health = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStaticsData().HealthDef, EvaluationParameters, Health);
	float CritRate = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStaticsData().CritRateDef, EvaluationParameters, CritRate);
	float CritDamage = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStaticsData().CritDamageDef, EvaluationParameters, CritDamage);
	float PunctureResistance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStaticsData().PunctureResistanceDef, EvaluationParameters, PunctureResistance);
	float ChopResistance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStaticsData().ChopResistanceDef, EvaluationParameters, ChopResistance);
	float LuntImpactResistance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStaticsData().LuntImpactResistanceDef, EvaluationParameters, LuntImpactResistance);
	float BreakingDefenseValue = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStaticsData().BreakingDefenseValueDef, EvaluationParameters, BreakingDefenseValue);
	float BaseDamage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName(TEXT("Fight.Damage"))),false);
	float TotalDamage=0;
	float Resistance=0;
	float CauseCritDamage = FMath::RandRange(0.f, 1.f) <= CritRate ? CritDamage : 1;
	if (EvaluationParameters.SourceTags->HasTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Fight.DamageType.Chop")))))
	{
		Resistance = ChopResistance;
	}
	if (EvaluationParameters.SourceTags->HasTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Fight.DamageType.LuntImpact")))))
	{
		Resistance = LuntImpactResistance;
	}
	if (EvaluationParameters.SourceTags->HasTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Fight.DamageType.Puncture")))))
	{
		Resistance = PunctureResistance;
	}
	TotalDamage = BaseDamage * (1 - Resistance / 100)* CauseCritDamage;
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStaticsData().HealthProperty, EGameplayModOp::Additive, -TotalDamage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStaticsData().BreakingDefenseValueProperty, EGameplayModOp::Additive, -TotalDamage));
}
