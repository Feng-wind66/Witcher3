// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VFX/Magic/MagicBase.h"
#include "Aard.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API AAard : public AMagicBase
{
	GENERATED_BODY()
public:
	virtual void OnMagicOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
};
