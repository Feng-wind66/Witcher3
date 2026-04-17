// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DarkeController.generated.h"
class ULootMenu;
/**
 * 
 */
UCLASS()
class WITCHER3_API ADarkeController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<ULootMenu>LootMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ULootMenu>LootMenuClass;
protected:
	void BeginPlay()override;
};
