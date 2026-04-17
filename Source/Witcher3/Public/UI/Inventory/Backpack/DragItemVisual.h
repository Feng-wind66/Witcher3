// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DragItemVisual.generated.h"
class UImage;
/**
 * 
 */
UCLASS()
class WITCHER3_API UDragItemVisual : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UTexture2D>ShowImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* VisualImage;
	UFUNCTION()
	void SetVisualImage();
};
