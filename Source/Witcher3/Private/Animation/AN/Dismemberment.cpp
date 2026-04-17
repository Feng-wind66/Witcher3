// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN/Dismemberment.h"
#include "Character/BaseCharacter.h"
void UDismemberment::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!IsValid(MeshComp))return;
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		Character->PreDismemberment(BoneName);
	}
}
