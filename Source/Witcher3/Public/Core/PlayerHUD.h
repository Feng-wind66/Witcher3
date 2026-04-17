// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"
class UMainUIComponent;
UCLASS()
class WITCHER3_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
public:
	APlayerHUD();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMainUIComponent* MainUIComponent;
};
