// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Enum/Enum.h"
#include "Components/ActorComponent.h"
#include "Interact.generated.h"
class USphereComponent;
class UCameraComponent;
class UMainUIComponent;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WITCHER3_API UInteract : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteract();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent>InteractiveSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInteracting;
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<AActor>>WeakOverlappingActors;
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<UCameraComponent>CameraComponent;
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<class AGameMode_1>WeakGameMode;
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AActor>WeakOwner;
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AActor>WeakCurrentInteractActor;
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<UMainUIComponent>WeakMainUIComponent;
	UFUNCTION(BlueprintCallable)
	void OnInteractiveSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnInteractiveSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	virtual void BeginPlay() override;
};
