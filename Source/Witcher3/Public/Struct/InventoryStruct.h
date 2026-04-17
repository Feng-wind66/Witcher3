#pragma once
#include"Enum/Enum.h"
#include "GameplayEffect.h"
#include "InventoryStruct.generated.h"
USTRUCT(BlueprintType)
struct FEquipmentEntryInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FActiveGameplayEffectHandle GEHandle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Describe;
};
USTRUCT(BlueprintType)
struct FEquipmentInfo
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EquipmentID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText EquipmentName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EquipmentLevel;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText EquipmentDescribe;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType EquipmentType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* EquipmentIcon;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> EquipmentPlayerBPClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> EquipmentEnemyBPClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FActiveGameplayEffectHandle EquipmentBurder;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemQuality EquipmentQuality;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TSubclassOf<UGameplayEffect>, FEquipmentEntryInfo>MainEntry;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TSubclassOf<UGameplayEffect>, FEquipmentEntryInfo>SubEntry;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TSubclassOf<UGameplayAbility>, FGameplayAbilitySpecHandle>UniqueEntry;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TSubclassOf<UGameplayAbility>, FGameplayAbilitySpecHandle>SetEffect;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Stack;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxStack;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Price;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemHandle=0;
    bool operator==(const FEquipmentInfo& Other) const
    {
        return ItemHandle == Other.ItemHandle;
    }
};