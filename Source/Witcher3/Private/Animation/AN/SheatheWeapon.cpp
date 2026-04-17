// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN/SheatheWeapon.h"
#include "Blueprint/Component/Action.h"
#include "Blueprint/Component/Equipment.h"
void USheatheWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		if (TObjectPtr<AActor>Owner = MeshComp->GetOwner())
		{
			if (TObjectPtr < UEquipment> Equipment = Owner->FindComponentByClass<UEquipment>())
			{
				Equipment->SheatheWeapon(WhichWeapon);
			}
			if (TObjectPtr < UAction>Action = Owner->FindComponentByClass<UAction>())
			{
				Action->SetCurrentActionState(ActionState);
			}
		}
	}
}
