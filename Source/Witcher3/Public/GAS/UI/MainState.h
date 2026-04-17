// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include"Enum/Enum.h"
#include "MainState.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API UMainState : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeMagicImageByType(EMagicType MagicType);
};
