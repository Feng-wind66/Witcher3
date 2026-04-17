// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DarkeState.generated.h"
class URetainInventoryData;
/**
 * 
 */
UCLASS()
class WITCHER3_API ADarkeState : public APlayerState
{
	GENERATED_BODY()
public:
	ADarkeState();
	int32 PlayerLevel;
	URetainInventoryData* RetainIventoryDataComponent;
protected:
	virtual void BeginPlay() override;
};
