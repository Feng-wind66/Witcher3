// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "BurnDamage.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API UBurnDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UBurnDamage();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
