
#pragma once

#include "CoreMinimal.h"
#include"Enum/Enum.h"
#include"Struct/EquipmentStruct.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class WITCHER3_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();
	void SetCollision();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent>WeaponSheath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent>MainWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageSource WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponInfo WeaponInfo;
protected:
	virtual void BeginPlay() override;
};
