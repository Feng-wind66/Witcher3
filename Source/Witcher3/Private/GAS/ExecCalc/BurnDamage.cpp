// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ExecCalc/BurnDamage.h"
#include"GAS/AS.h"
struct DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAS, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAS, MaxHealth, Target, true);
	}
};
static const DamageStatics& DamageStaticData()
{
	static DamageStatics DmgStatics;
	return DmgStatics;
}
UBurnDamage::UBurnDamage()
{
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().MaxHealthDef);
}

void UBurnDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	 UAbilitySystemComponent*TargetASC=ExecutionParams.GetTargetAbilitySystemComponent();
	 if (!IsValid(TargetASC))return;
	 const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	 FAggregatorEvaluateParameters EvaluationParameters;
	 EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	 EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	 float BurnDamage = 0.f;
	 FGameplayTag BurnDamageTag=FGameplayTag::RequestGameplayTag(FName(TEXT("BurnDamage")));
	 BurnDamage = Spec.GetSetByCallerMagnitude(BurnDamageTag,false,-1);
	 if (BurnDamage < 0)
	 {
		 float TargetMaxHealth = 0;
		 if (ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MaxHealthDef, EvaluationParameters, TargetMaxHealth))
		 {
			 BurnDamage = 0.01f * TargetMaxHealth ;
		 }
	 }
	 if (BurnDamage > 0.f)
	 {
		 int32 CurrentStackCount = Spec.GetStackCount();
		 BurnDamage *= CurrentStackCount;
		 OutExecutionOutput.AddOutputModifier(
			 FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -BurnDamage)
		 );
	 }
}
