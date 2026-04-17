// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include"Enum/Enum.h"
#include "SheatheWeapon.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API USheatheWeapon : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EActionState ActionState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageSource WhichWeapon;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
