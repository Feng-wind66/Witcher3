// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DarkeState.h"
#include "Blueprint/Component/RetainInventoryData.h"

ADarkeState::ADarkeState()
{
	RetainIventoryDataComponent = CreateDefaultSubobject<URetainInventoryData>(TEXT("RetainInventoryDataComponent"));
}

void ADarkeState::BeginPlay()
{
	Super::BeginPlay();
	
}
