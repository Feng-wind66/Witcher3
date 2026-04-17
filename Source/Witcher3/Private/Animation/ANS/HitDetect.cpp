// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS/HitDetect.h"
#include "Equipment/WeaponBase.h"
#include "Blueprint/Component/Action.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/Component/Equipment.h"
#include "Blueprint/Interface/CharacterInterface.h"
void UHitDetect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr)
	{
		if (Owner->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			if (UEquipment* EquipmentComponent = ICharacterInterface::Execute_GetEquipmentComponent(Owner))
			{
				EquipmentComponent->InitHitDetectLocations();
				if (AWeaponBase* CurrentMainWeaponRef = EquipmentComponent->CurrentMainWeaponRef)
				{
					if (UAction* Action = ICharacterInterface::Execute_GetActionComponent(Owner))
					{
						if (Action->CurrentActionInfo.ActionType.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.LightAttack")))))
						{
							UGameplayStatics::PlaySoundAtLocation(CurrentMainWeaponRef, CurrentMainWeaponRef->WeaponInfo.WeaponLightAttackWooshSound, CurrentMainWeaponRef->MainWeapon->GetSocketLocation(FName(TEXT("SOCKET_WeaponTraceEnd"))));
						}
						else
						{
							UGameplayStatics::PlaySoundAtLocation(CurrentMainWeaponRef, CurrentMainWeaponRef->WeaponInfo.WeaponHeavyAttackWooshSound, CurrentMainWeaponRef->MainWeapon->GetSocketLocation(FName(TEXT("SOCKET_WeaponTraceEnd"))));
						}
					}
				}
			}
		}
	}
}

void UHitDetect::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr)
	{
		if (Owner->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			if (UEquipment* EquipmentComponent = ICharacterInterface::Execute_GetEquipmentComponent(Owner))
			{
				EquipmentComponent->HitDetect();
			}
		}
	}
}

void UHitDetect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr)
	{
		if (Owner->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			if (UEquipment* EquipmentComponent = ICharacterInterface::Execute_GetEquipmentComponent(Owner))
			{
				EquipmentComponent->ClearHittedActors();
			}
		}
	}
}
