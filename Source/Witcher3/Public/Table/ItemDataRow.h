#pragma once
#include "Engine/DataTable.h"
#include"Enum/Enum.h"
#include "ItemDataRow.generated.h"
USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemLevel;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemQuality ItemQuality;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ItemDescribe;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType ItemType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D *ItemIcon;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor>ItemPlayerBPClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor>ItemEnemyBPClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Price;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ItemBurden;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinEntryCount;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxEntryCount;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinEntryLevel;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxEntryLevel;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UniqueEntryID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SetEffectID;
};