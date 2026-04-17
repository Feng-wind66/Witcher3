// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DescribeText.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API UDescribeText : public UCommonUserWidget
{
	GENERATED_BODY()
public:
protected:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void InitText(const FText &Text);
};
