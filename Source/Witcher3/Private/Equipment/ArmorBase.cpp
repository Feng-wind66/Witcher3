// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/ArmorBase.h"

// Sets default values
AArmorBase::AArmorBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ArmorSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmorSkeletalMesh"));
}

void AArmorBase::BeginPlay()
{
	Super::BeginPlay();
}

void AArmorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

