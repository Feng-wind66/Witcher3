// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN/CanCombo.h"
#include"Enum/Enum.h"
#include "Blueprint/Component/Action.h"
void UCanCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation,EventReference);
	if (MeshComp)
	{
		if (AActor*Owner = MeshComp->GetOwner())
		{
			if (UAction* ActionRef =Owner->FindComponentByClass<UAction>())
			{
				ActionRef->TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.CanCombo"))),EPriority::EP_Supreme);
				return;
			}
		}
	}
}
