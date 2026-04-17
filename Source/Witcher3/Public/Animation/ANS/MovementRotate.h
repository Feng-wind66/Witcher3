#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MovementRotate.generated.h"

class UCharacterMovementComponent;
UCLASS()
class WITCHER3_API UMovementRotate : public UAnimNotifyState
{
    GENERATED_BODY()

public:
    //virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
    //virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,const FAnimNotifyEventReference& EventReference) override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
    float InterpSpeed = 1.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
    TSubclassOf<UCharacterMovementComponent> MovementComponentClass;
};