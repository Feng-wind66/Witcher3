// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmorBase.generated.h"

UCLASS()
class WITCHER3_API AArmorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AArmorBase();
    virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent>ArmorSkeletalMesh;
protected:
	virtual void BeginPlay() override;

};
