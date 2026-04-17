// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Enum\Enum.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include "GameMode_1.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API AGameMode_1 : public AGameModeBase,public ICharacterInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void SetPlayerRef(APawn*PawnRef);
protected:
	/*
	PROPERTY
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EActionState CurrentActionState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APawn* PlayerPawnRef;
	/*
	FUNCTION
	*/
	virtual EActionState GetCurrentActionState_Implementation()override;
	virtual APawn* GetPlayerRef_Implementation()override;
	virtual void ChangeCurrentActionState_Implementation(EActionState ActionState)override;
	
};
