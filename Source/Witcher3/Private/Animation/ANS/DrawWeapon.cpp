// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS/DrawWeapon.h"
#include "Blueprint/Component/Action.h"
#include "Blueprint/Component/Equipment.h"

void UDrawWeapon::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			if (UEquipment* Equipment = Owner->FindComponentByClass<UEquipment>())
			{
				Equipment->DrawWeapon(WhichWeapon);
			}
			if(UAction* Action = Owner->FindComponentByClass<UAction>())
			{
				Action->SetCurrentActionState(ActionState);
			}
		}
	}
}
