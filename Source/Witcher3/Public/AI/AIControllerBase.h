// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
public:
	FTimerHandle InitHandle;
	virtual void OnPossess(APawn* InPawn) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TWeakObjectPtr<class UAction>ActionComponent;
};
