// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/Component/RetainInventoryData.h"

URetainInventoryData::URetainInventoryData()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URetainInventoryData::BeginPlay()
{
	Super::BeginPlay();
}

void URetainInventoryData::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

