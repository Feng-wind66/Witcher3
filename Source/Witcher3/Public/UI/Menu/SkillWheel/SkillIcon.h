// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SkillIcon.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API USkillIcon : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UTexture2D>NormalSkillIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D>OnHoveredSkillIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillSize;
	UFUNCTION(BlueprintImplementableEvent)
	void OnHovered();
	UFUNCTION(BlueprintImplementableEvent)
	void UnHovered();
};
