// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VFX/Magic/MagicBase.h"
#include "Igni.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API AIgni : public AMagicBase
{
	GENERATED_BODY()
public:
	virtual void OnMagicOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class UNiagaraSystem>HittedFireVFX;
};
