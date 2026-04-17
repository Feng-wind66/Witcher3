// Fill out your copyright notice in the Description page of Project Settings.


#include "VFX/Magic/Quen.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Blueprint/Component/Equipment.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

void AQuen::BeginPlay()
{
	Super::BeginPlay();
	if (!Releaser.IsValid() || !ReleaserASC.IsValid()||!Cast<ICharacterInterface>(Releaser))return;
	AttachToActor(Releaser.Get(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName(TEXT("spine_03")));
	UEquipment* EquipmentComponent = ICharacterInterface::Execute_GetEquipmentComponent(Releaser.Get());
	if (!IsValid(EquipmentComponent))return;
	if (AllGE && AllGE->GE_Quen)
	{
		FGameplayEffectContextHandle Context = ReleaserASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = ReleaserASC->MakeOutgoingSpec(AllGE->GE_Quen, 1.0f, Context);
		QuenHandle=ReleaserASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		EquipmentComponent->QuenActor = this;
	}
}

void AQuen::Destroyed()
{
	if (Releaser.IsValid())
	{
		UGameplayStatics::PlaySoundAtLocation(this, BreakShound, Releaser->GetActorLocation());
		if (USkeletalMeshComponent* Mesh = Cast<ACharacter>(Releaser) ? Cast<ACharacter>(Releaser)->GetMesh() : nullptr)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				BreakNiagara,
				Mesh,
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true
			);
		}
	}
	if (ReleaserASC.IsValid())
	{
		ReleaserASC->RemoveActiveGameplayEffect(QuenHandle);
	}
	Super::Destroyed();
}
