#pragma once
#include "Engine/DataTable.h"
#include "StatsTableRow.generated.h"

USTRUCT(BlueprintType)
struct FStatsDataRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool CanStack;
};