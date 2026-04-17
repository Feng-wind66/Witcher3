// Fill out your copyright notice in the Description page of Project Settings.


#include "VFX/Magic/MagicBase.h"
#include "NiagaraSystem.h"
#include"Macro/Macro.h"
#include "NiagaraComponent.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
AMagicBase::AMagicBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(GetRootComponent());
	CollisionBox->SetupAttachment(NiagaraComponent);
	CollisionBox->InitBoxExtent(BoxSize);
}

void AMagicBase::OnMagicOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AMagicBase::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(ActorLifeSpan);
	Releaser = GetOwner();
	if (IsValid(CollisionBox))
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMagicBase::OnMagicOverlap);
		DELAY(GetWorld()->GetTimerManager(), this, DisableCollisionTimerHandle,BoxLifeSpan, &AMagicBase::DisableCollision);
	}
	ReleaserASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Releaser.Get());
	if (!ReleaserASC.IsValid())return;
	if (AllGE && AllGE->GE_EnergyCost)
	{
		FGameplayEffectContextHandle Context = ReleaserASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = ReleaserASC->MakeOutgoingSpec(AllGE->GE_EnergyCost, 1.0f, Context);
		if (SpecHandle.Data.IsValid())
		{
			FGameplayTag CostTag = FGameplayTag::RequestGameplayTag(FName("Data.Cost.Energy"));
			SpecHandle.Data->SetSetByCallerMagnitude(CostTag, -MagicBaseCost);
			ReleaserASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		}
	}
	if (AllGE && AllGE->GE_Tired)
	{
		FGameplayEffectSpecHandle SpecHandle = ReleaserASC->MakeOutgoingSpec(AllGE->GE_Tired, 1.0f, ReleaserASC->MakeEffectContext());
		if (SpecHandle.Data.IsValid())
		{
			ReleaserASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		}
	}
	UGameplayStatics::PlaySoundAtLocation(Owner, MagicSoundMap.Find(MagicType) ? *MagicSoundMap.Find(MagicType) : nullptr, Owner->GetActorLocation(), 1, 1, GetStartTimeByMagicType(MagicType));
}

void AMagicBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicBase::DisableCollision()
{
	CollisionBox->DestroyComponent();
}
float AMagicBase::GetStartTimeByMagicType(EMagicType Type)
{
	switch (Type)
	{
	case EMagicType::EMT_Axii:return 0.4;
		break;
	case EMagicType::EMT_Yrden:return 0.2;
		break;
	}
	return 0;
}