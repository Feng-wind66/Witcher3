// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS/MotionWarping.h"
#include "Blueprint/Component/Action.h"
#include "Blueprint/Interface/CharacterInterface.h"
void UMotionWarping::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr)
	{
		if (Owner->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			if (UAction* Action = ICharacterInterface::Execute_GetActionComponent(Owner))
			{
				Action->GetMotionWarpingLocationAndRotation();
			}
		}
	}
}

void UMotionWarping::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr)
	{
		if (Owner->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			if (UAction* Action = ICharacterInterface::Execute_GetActionComponent(Owner))
			{
				if (bDurationMotionWarping)
				{
					Action->GetMotionWarpingLocationAndRotation();
				}
				Action->MotionWarping();
			}
		}
	}
}

void UMotionWarping::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr)
	{
		if (Owner->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			if (UAction* Action = ICharacterInterface::Execute_GetActionComponent(Owner))
			{
				Action->ResetMotionWarping();
			}
		}
	}
}
