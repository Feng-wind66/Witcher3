#pragma once
#include "Engine/DataTable.h"
#include"Enum/Enum.h"
#include "GameplayEffect.h"
#include"EntryDataRow.generated.h"
USTRUCT(BlueprintType)
struct FEntryDataRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemQuality Rarity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UGameplayEffect>Attribute;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Min;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Max;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Describe;
};