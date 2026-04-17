// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RetainInventoryData.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WITCHER3_API URetainInventoryData : public UActorComponent
{
	GENERATED_BODY()

public:	
	URetainInventoryData();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetNewItemHandle() { return ++ItemHandle;};
protected:
	virtual void BeginPlay() override;
	
private:
	int32 ItemHandle=0;
};
