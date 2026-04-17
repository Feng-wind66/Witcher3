// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/BaseGameplayAbility.h"
void UBaseGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect*CoolDownGE=GetCooldownGameplayEffect();
	if (!IsValid(CoolDownGE))return;
	static const FGameplayTag CooldownDataTag = FGameplayTag::RequestGameplayTag(FName("Data.Cooldown"));
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CoolDownGE->GetClass(), GetAbilityLevel());
	if (SpecHandle.Data.IsValid())
	{
		float BaseCooldownDuration = CooldownDuration.GetValueAtLevel(GetAbilityLevel());
		float FinalCooldownDuration = BaseCooldownDuration;
		//往后可以加一些修改冷却时间的逻辑
		SpecHandle.Data->DynamicGrantedTags.AppendTags(CooldownTags);
		SpecHandle.Data->SetSetByCallerMagnitude(CooldownDataTag,FinalCooldownDuration);
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

void UBaseGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (!IsValid(CostGE))return;
	static const FGameplayTag CostDataTag = FGameplayTag::RequestGameplayTag(FName("Data.Cost"));
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CostGE->GetClass(), GetAbilityLevel());
	if (SpecHandle.Data.IsValid())
	{
		float BaseCost = CostAmount.GetValueAtLevel(GetAbilityLevel());
		float FinalCost = BaseCost;
		//往后可以加一些修改消耗的逻辑
		SpecHandle.Data->SetSetByCallerMagnitude(CostDataTag, -FinalCost);
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

const FGameplayTagContainer* UBaseGameplayAbility::GetCooldownTags() const
{
	return &CooldownTags;
}
