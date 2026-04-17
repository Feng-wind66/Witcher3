// Fill out your copyright notice in the Description page of Project Settings.


#include "VFX/Magic/Igni.h"
#include "Blueprint/Component/Action.h"
#include "GAS/ASC.h"
#include "GameFramework/Character.h"
#include "Blueprint/Interface/TargettingInterface.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include"Macro/Macro.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
void AIgni::OnMagicOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == Releaser) return;
	if (!Cast<ITargettingInterface>(OtherActor) || !Cast<ICharacterInterface>(OtherActor) || !Releaser.IsValid())return;
	UAction* ActionComponent = ICharacterInterface::Execute_GetActionComponent(OtherActor);
	UASC* ReleaserSpecificASC = Cast<UASC>(ReleaserASC);
	if (!IsValid(ActionComponent)||!ReleaserASC.IsValid())return;
	if (ITargettingInterface::Execute_IsCombatDamageble(OtherActor) && ITargettingInterface::Execute_IsCombatTargettable(OtherActor))
	{
		if (!HittedActors.Contains(OtherActor))
		{
			HittedActors.AddUnique(OtherActor);
			ReleaserSpecificASC->DealWithDamage(OtherActor, FGameplayTag::RequestGameplayTag(FName("Action.Combat.Magic")), EDamageSource::EDS_Magic);
			ActionComponent->EndParry();
			ActionComponent->PlayHitAnim(Releaser.Get(), EWhichDamageType::EWDT_Magic_Igni, false);
			ACharacter* Character = Cast<ACharacter>(OtherActor);
			if (USkeletalMeshComponent* Mesh = IsValid(Character) ? Character->GetMesh() : nullptr)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(
					HittedFireVFX,                 
					Mesh,           
					FName("spine_03"),    
					FVector(),
					FRotator(),
					EAttachLocation::SnapToTarget, 
					true                    
				);
			}
		}
	}
}