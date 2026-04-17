// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ItemDescribe.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API UItemDescribe : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void AddItemDescribe(const FText &Describe, bool IsTitle);
protected:
	
};
