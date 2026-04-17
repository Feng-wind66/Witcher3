// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponSheath = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponSheath"));
	MainWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainWeapon"));
	SetRootComponent(WeaponSheath);
	MainWeapon->SetupAttachment(WeaponSheath);
	SetCollision();
}

void AWeaponBase::SetCollision()
{
	WeaponSheath->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponSheath->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	WeaponSheath->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	WeaponSheath->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Ignore);
	WeaponSheath->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel2, ECollisionResponse::ECR_Ignore);
	MainWeapon->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MainWeapon->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	MainWeapon->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	MainWeapon->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Ignore);
	MainWeapon->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel2, ECollisionResponse::ECR_Overlap);
	MainWeapon->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

