// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN/FinisherComplete.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include "Blueprint/Component/Action.h"
void UFinisherComplete::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!IsValid(MeshComp)||!Cast<ICharacterInterface>(MeshComp->GetOwner()))return;
	if (UAction* Action = ICharacterInterface::Execute_GetActionComponent(MeshComp->GetOwner()))
	{
		Action->SetCurrentActionState(EActionState::EAS_InCombat);
	}
}
