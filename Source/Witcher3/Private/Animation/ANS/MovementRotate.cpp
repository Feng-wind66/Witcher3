#include "Animation/ANS/MovementRotate.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
void UMovementRotate::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
    if (MeshComp && MeshComp->GetOwner())
    {
        if (UCharacterMovementComponent* OwnerMovementComponent = MeshComp->GetOwner()->FindComponentByClass<UCharacterMovementComponent>())
        {
            AActor* Owner = MeshComp->GetOwner();
            FVector InputVector = OwnerMovementComponent->GetLastInputVector();
            if (InputVector.SizeSquared() > KINDA_SMALL_NUMBER)
            {
                FRotator TargetRotation = FRotationMatrix::MakeFromX(InputVector.GetSafeNormal()).Rotator();
                FRotator NewRot = UKismetMathLibrary::RInterpTo(Owner->GetActorRotation(), TargetRotation, FrameDeltaTime, InterpSpeed);
                Owner->SetActorRotation(NewRot);
            }
        }
    }
}