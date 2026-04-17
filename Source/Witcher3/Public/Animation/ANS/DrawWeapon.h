// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Enum/Enum.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DrawWeapon.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API UDrawWeapon : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EActionState ActionState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageSource WhichWeapon;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
};
