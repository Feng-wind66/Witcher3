// Fill out your copyright notice in the Description page of Project Settings.


#include "VFX/Magic/Yrden.h"
#include "Blueprint/Interface/TargettingInterface.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include "NiagaraComponent.h"
#include"Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AYrden::AYrden()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(NiagaraComponent);
	SetActorTickInterval(1.f);
	SphereComponent->InitSphereRadius(325.f);
}

void AYrden::ApplySlow(UAbilitySystemComponent* HittedASC,AActor*Hitted)
{
	if (!IsValid(HittedASC) || !ReleaserASC.IsValid()) return;
	FGameplayEffectContextHandle Context = ReleaserASC->MakeEffectContext();
	Context.AddInstigator(Releaser.Get(), this);
	FGameplayEffectSpecHandle SpecHandle = ReleaserASC->MakeOutgoingSpec(AllGE->GE_Slow, 1.0f, Context);
	if (SpecHandle.Data.IsValid())
	{
		FGameplayTag SlowDurationTag = FGameplayTag::RequestGameplayTag(FName("Data.Duration.Slow"));
		SpecHandle.Data->SetSetByCallerMagnitude(SlowDurationTag, SlowDuration);
		FActiveGameplayEffectHandle AppliedHandle = ReleaserASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, HittedASC);
		if (AppliedHandle.WasSuccessfullyApplied())
		{
			ActiveSlowHandles.Add(TWeakObjectPtr<AActor>(Hitted), AppliedHandle);
		}
	}
}

void AYrden::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == Releaser) return;
	if (!Cast<ITargettingInterface>(OtherActor) || !Cast<ICharacterInterface>(OtherActor) || !Releaser.IsValid())return;
	if (ITargettingInterface::Execute_IsCombatDamageble(OtherActor) && ITargettingInterface::Execute_IsCombatTargettable(OtherActor))
	{
		if (!HittedActors.Contains(OtherActor))
		{
			ApplySlow(OtherActor ? UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor) : nullptr,OtherActor);
			HittedActors.AddUnique(OtherActor);
			ICharacterInterface::Execute_SetCurrentLocomotionState(OtherActor, ELocomotionState::ELS_Walk);
		}
	}
}

void AYrden::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == Releaser) return;
	UAbilitySystemComponent*HittedASC = OtherActor ? UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor) : nullptr;
	if (!Cast<ITargettingInterface>(OtherActor) || !Cast<ICharacterInterface>(OtherActor) || !Releaser.IsValid()||!IsValid(HittedASC)||!AllGE||!AllGE->GE_Slow)return;
	if (ITargettingInterface::Execute_IsCombatDamageble(OtherActor) && ITargettingInterface::Execute_IsCombatTargettable(OtherActor))
	{
		TWeakObjectPtr<AActor> WeakTarget(OtherActor);
		if (FActiveGameplayEffectHandle* HandlePtr = ActiveSlowHandles.Find(WeakTarget))
		{
			if (WeakTarget.IsValid() && HandlePtr->IsValid())
			{
				HittedASC->RemoveActiveGameplayEffect(*HandlePtr, 1);
			}
			ActiveSlowHandles.Remove(WeakTarget);
		}
		ICharacterInterface::Execute_SetCurrentLocomotionState(OtherActor, ELocomotionState::ELS_Walk);
		HittedActors.Remove(OtherActor);
	}
}

void AYrden::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(SphereComponent))
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AYrden::OnSphereOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AYrden::OnSphereEndOverlap);
	}
}
