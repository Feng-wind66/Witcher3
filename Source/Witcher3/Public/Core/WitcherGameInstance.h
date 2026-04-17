// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WitcherGameInstance.generated.h"
class UStartMenu;
/**
 * 
 */
UCLASS()
class WITCHER3_API UWitcherGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStartMenu>StartMenu;
};
