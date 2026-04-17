// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/Component/Interact.h"
#include"Components/SphereComponent.h"
#include "UI/Main/MainUI.h"
#include "UI/Component/MainUIComponent.h"
#include "FunctionLibrary/CommonFuction.h"
#include "Camera/CameraComponent.h"   
#include "Blueprint/Interface/CharacterInterface.h"
#include "Blueprint/Interface/InteractableInterface.h"
#include "Core/GameMode_1.h"

UInteract::UInteract()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractiveSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractiveSphere"));
	InteractiveSphere->SetComponentTickInterval(0.2f);
}


void UInteract::BeginPlay()
{
	Super::BeginPlay();
	WeakOwner = GetOwner();
	WeakMainUIComponent = UCommonFuction::GetMainUIComponent(this);
	if (IsValid(GetWorld()))
	{
		WeakGameMode = Cast<AGameMode_1>(GetWorld()->GetAuthGameMode());
	}
	if (IsValid(InteractiveSphere)&& WeakOwner.IsValid())
	{
		InteractiveSphere->AttachToComponent(WeakOwner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		InteractiveSphere->OnComponentBeginOverlap.AddDynamic(this, &UInteract::OnInteractiveSphereBeginOverlap);
		InteractiveSphere->OnComponentEndOverlap.AddDynamic(this, &UInteract::OnInteractiveSphereEndOverlap);
	}
	if (WeakOwner.IsValid())
	{
		CameraComponent = WeakOwner->FindComponentByClass<UCameraComponent>();
	}
}


void UInteract::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!CameraComponent.IsValid()||!WeakOwner.IsValid()||!WeakMainUIComponent.IsValid()||!IsValid(WeakMainUIComponent->MainUIRef))return;
	if (bIsInteracting&&WeakCurrentInteractActor.IsValid())
	{
		WeakMainUIComponent->MainUIRef->SetInteractButtonVisibility(false);
		return;
	}
	float HighestScore = INT32_MIN;
	AActor* LastBestActor = nullptr;
	AActor* BestActor = nullptr;
	for (auto WeakActor : WeakOverlappingActors)
	{
		if (WeakActor.IsValid()&& IInteractableInterface::Execute_CanBeInteracted(WeakActor.Get(), WeakOwner.Get()))
		{
			float Angle=FVector::DotProduct(WeakActor->GetActorLocation()- CameraComponent->GetComponentLocation(),CameraComponent->GetForwardVector());
			if (Angle < 0)continue;
			float Distance = FVector::Distance(WeakActor->GetActorLocation(), WeakOwner->GetActorLocation());
			if (Angle * 500 - Distance > HighestScore)
			{
				HighestScore = Angle * 500 - Distance;
				BestActor = WeakActor.Get();
			}
		}
	}
	WeakCurrentInteractActor = BestActor;
	FText ShowText = WeakCurrentInteractActor.IsValid()?IInteractableInterface::Execute_GetInteractionText(WeakCurrentInteractActor.Get()):FText();
	WeakOverlappingActors.IsEmpty() ? WeakMainUIComponent->MainUIRef->SetInteractButtonVisibility(false) : WeakMainUIComponent->MainUIRef->SetInteractButtonVisibility(true, WeakOwner.Get(), ShowText);
}

void UInteract::OnInteractiveSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<IInteractableInterface>(OtherActor)||!WeakOwner.IsValid())return;
	if (!IInteractableInterface::Execute_CanBeInteracted(OtherActor, WeakOwner.Get()))return;
	WeakOverlappingActors.AddUnique(OtherActor);
}

void UInteract::OnInteractiveSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Cast<IInteractableInterface>(OtherActor) || !WeakOwner.IsValid())return;
	if (WeakCurrentInteractActor == OtherActor)
	{
		WeakCurrentInteractActor = nullptr;
		IInteractableInterface::Execute_EndInteract(OtherActor, WeakOwner.Get());
	}
	WeakOverlappingActors.Remove(OtherActor);
}
