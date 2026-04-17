// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

UCLASS()
class WITCHER3_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cooldown")
    FScalableFloat CooldownDuration;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cooldown")
    FGameplayTagContainer CooldownTags;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cost")
    FScalableFloat CostAmount;
protected:
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
    virtual const FGameplayTagContainer* GetCooldownTags() const override;
};
