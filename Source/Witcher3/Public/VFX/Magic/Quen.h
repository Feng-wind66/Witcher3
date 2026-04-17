// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VFX/Magic/MagicBase.h"
#include "Quen.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API AQuen : public AMagicBase
{
	GENERATED_BODY()
public:
	virtual void Destroyed() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraSystem>BreakNiagara;//À¶Í¼¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase>BreakShound;//À¶Í¼¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActiveGameplayEffectHandle QuenHandle;
protected:
	void BeginPlay() override;
};
