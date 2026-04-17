// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DragDropFunction.generated.h"
class UBackpackGrid;
/**
 * 
 */
UCLASS()
class WITCHER3_API UDragDropFunction : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBackpackGrid* DragGrid;
};
