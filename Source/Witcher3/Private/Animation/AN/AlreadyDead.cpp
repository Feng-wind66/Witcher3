// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN/AlreadyDead.h"
#include "Character/BaseCharacter.h"
void UAlreadyDead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!IsValid(MeshComp))return;
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		Character->Tags.Add(FName(TEXT("Death")));
		Character->OnCharacterDeathEvent.Broadcast(Character);
	}
}
