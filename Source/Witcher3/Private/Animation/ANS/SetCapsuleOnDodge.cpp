// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS/SetCapsuleOnDodge.h"
#include "Character/BaseCharacter.h"

void USetCapsuleOnDodge::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	ABaseCharacter* BaseCharacter = IsValid(MeshComp) ? Cast<ABaseCharacter>(MeshComp->GetOwner()) : nullptr;
	UAnimInstance* AnimInstance = IsValid(MeshComp) ? MeshComp->GetAnimInstance() : nullptr;
	if (IsValid(BaseCharacter)&&IsValid(AnimInstance))
	{
		BaseCharacter->Rolling(AnimInstance->GetCurveValue(TEXT("DodgeRoll")));
	}
}

void USetCapsuleOnDodge::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	ABaseCharacter* BaseCharacter = IsValid(MeshComp) ? Cast<ABaseCharacter>(MeshComp->GetOwner()) : nullptr;
	if (IsValid(BaseCharacter))
	{
		BaseCharacter->EndRoll();
	}
}
