// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy/BasicEnemy.h"
#include "Core/DarkeController.h"
#include"Macro/Macro.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameMode_1.h"
#include "UI/Menu/LootMenu.h"
#include "Blueprint/Component/Interact.h"
#include "Blueprint/Component/Inventory.h"
#include "Blueprint/Component/Action.h"

void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
	WeakDarkeController = Cast<ADarkeController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ABasicEnemy::Interact_Implementation(AActor* Interactor)
{
	if (!IsValid(Interactor))return;
	UInteract* InteractComponent = Interactor->FindComponentByClass<UInteract>();
	if (IsValid(InteractComponent))
	{
		InteractComponent->bIsInteracting = true;
	}
	if (IsValid(Action)&&IsValid(Inventory)&&WeakDarkeController.IsValid()&& WeakDarkeController->LootMenu&&IsValid(GameMode))
	{
		WeakDarkeController->LootMenu->UpdateDropItemInfo(Inventory->DropItemInfos,Interactor,this);
		WeakDarkeController->LootMenu->AddToViewport();
		WeakDarkeController->LootMenu->ActivateWidget();
		WeakDarkeController->SetShowMouseCursor(true);
		WeakDarkeController->SetInputMode(FInputModeGameAndUI());
		int32 ViewportSizeX, ViewportSizeY;
		WeakDarkeController->GetViewportSize(ViewportSizeX, ViewportSizeY);
		WeakDarkeController->SetMouseLocation(ViewportSizeX / 2, ViewportSizeY / 2);
	}
}

void ABasicEnemy::EndInteract_Implementation(AActor* Interactor)
{
	if (!IsValid(Interactor))return;
	UInteract* InteractComponent = Interactor->FindComponentByClass<UInteract>();
	if (IsValid(InteractComponent))
	{
		InteractComponent->bIsInteracting = false;
		if (!IInteractableInterface::Execute_CanBeInteracted(this, Interactor))
		{
			InteractComponent->WeakOverlappingActors.Remove(this);
		}
	}
	if (IsValid(Action) && WeakDarkeController.IsValid() && WeakDarkeController->LootMenu&&IsValid(GameMode))
	{
		WeakDarkeController->LootMenu->DeactivateWidget();
		WeakDarkeController->LootMenu->RemoveFromParent();
		WeakDarkeController->SetShowMouseCursor(false);
		WeakDarkeController->SetInputMode(FInputModeGameOnly());
	}
}

bool ABasicEnemy::CanBeInteracted_Implementation(AActor* Interactor)const
{
	return ActorHasTag("Death") && IsValid(Inventory) ? Inventory->DropItemInfos.Num() > 0:false;
}

FText ABasicEnemy::GetInteractionText_Implementation() const
{
	return InteractionText;
}
