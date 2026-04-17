// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VFX/Magic/MagicBase.h"
#include "GAS/AllGamePlayEffectAsset.h"
#include"Enum/Enum.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ReleaseMagic.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API UReleaseMagic : public UAnimNotify
{
	GENERATED_BODY()
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AMagicBase> Aard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMagicBase> Axii;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMagicBase> Igni;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMagicBase> Quen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMagicBase> Yrden;
	/*
	FUNCTION
	*/
	UFUNCTION(BlueprintCallable)
	TSubclassOf<AMagicBase> GetMagicClassByMagicType(EMagicType MagicType);
};
