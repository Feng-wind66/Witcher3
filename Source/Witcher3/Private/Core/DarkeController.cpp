// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DarkeController.h"
#include "UI/Menu/LootMenu.h"
void ADarkeController::BeginPlay()
{
	Super::BeginPlay();
	LootMenu=CreateWidget<ULootMenu>(GetWorld(), LootMenuClass);
}
