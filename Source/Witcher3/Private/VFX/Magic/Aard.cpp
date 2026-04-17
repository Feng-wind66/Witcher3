// Fill out your copyright notice in the Description page of Project Settings.


#include "VFX/Magic/Aard.h"
#include "Blueprint/Component/Action.h"
#include "Blueprint/Interface/TargettingInterface.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include"Macro/Macro.h"
void AAard::OnMagicOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == Releaser) return;
	if (!Cast<ITargettingInterface>(OtherActor)||!Cast<ICharacterInterface>(OtherActor)||!Releaser.IsValid())return;
	UAction* ActionComponent = ICharacterInterface::Execute_GetActionComponent(OtherActor);
	if (!IsValid(ActionComponent))return;
	if (ITargettingInterface::Execute_IsCombatDamageble(OtherActor) && ITargettingInterface::Execute_IsCombatTargettable(OtherActor))
	{
		if (!HittedActors.Contains(OtherActor))
		{
			HittedActors.AddUnique(OtherActor);
			ActionComponent->EndParry();
			ActionComponent->PlayHitAnim(Releaser.Get(), EWhichDamageType::EWDT_Magic_Aard, false);
		}
	}
}
