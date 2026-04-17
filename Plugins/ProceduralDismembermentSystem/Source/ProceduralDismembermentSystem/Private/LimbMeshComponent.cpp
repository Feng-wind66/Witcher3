// Copyright to Benoit L'hoest 2025


#include "LimbMeshComponent.h"


#include "Engine/SkeletalMesh.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "PhysicsEngine/PhysicsConstraintTemplate.h"

#include "Engine.h"
#include "BodySetupCore.h"
#include "GroomComponent.h"
#include "Physics/Experimental/PhysScene_Chaos.h"


void ULimbMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);
}

void ULimbMeshComponent::SetBoneTransformByName(FName BoneName, const FTransform& InTransform, EBoneSpaces::Type BoneSpace)
{

	if (!GetSkinnedAsset())
	{
		return;
	}

	check(!LeaderPoseComponent.IsValid()); //Shouldn't call set bone functions when we are using LeaderPoseComponent
	
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	int32 BoneIndex = GetBoneIndex(BoneName);
	if (BoneIndex >= 0 && BoneIndex < BoneSpaceTransforms.Num())
	{
		
		BoneSpaceTransforms[BoneIndex] = InTransform;
		

		// If we haven't requested local space we need to transform the position passed in
		// if(BoneSpace != EBoneSpaces::LocalSpace)
		{
			if (BoneSpace == EBoneSpaces::WorldSpace)
			{
				BoneSpaceTransforms[BoneIndex].SetToRelativeTransform(GetComponentToWorld());
			}
			
		
			GetSkeletalMeshAsset()->FillComponentSpaceTransforms(BoneSpaceTransforms, FillComponentSpaceTransformsRequiredBones, GetEditableComponentSpaceTransforms());
			bNeedToFlipSpaceBaseBuffers = true; // Have updated space bases so need to flip
			FlipEditableSpaceBases();

			MarkRefreshTransformDirty();
			
		}
	}
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void ULimbMeshComponent::MarkRefreshTransformDirty()
{
	bNeedsRefreshTransform = true;
}

bool ULimbMeshComponent::ShouldUpdateTransform(bool bLODHasChanged) const
{
	// we don't always update transform - each function when they changed will update
	return Super::ShouldUpdateTransform(bLODHasChanged) || bNeedsRefreshTransform;
}

void ULimbMeshComponent::SetBoneScaleByName(FName BoneName, FVector InScale3D, EBoneSpaces::Type BoneSpace)
{

	int32 BoneIndex = GetBoneIndex(BoneName);

	if (BoneIndex >= 0 && BoneIndex < BoneSpaceTransforms.Num())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		FTransform CurrentTransform = BoneSpaceTransforms[BoneIndex];
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
		CurrentTransform.SetScale3D(InScale3D);


		SetBoneTransformByName(BoneName, CurrentTransform, EBoneSpaces::Type::ComponentSpace);
	}
}

void ULimbMeshComponent::FillComponentSpaceTransforms()
{
	//ANIM_MT_SCOPE_CYCLE_COUNTER(FillComponentSpaceTransforms, IsRunningParallelEvaluation());

	if (!GetSkinnedAsset())
	{
		return;
	}

	// right now all this does is to convert to SpaceBases
	check(GetSkinnedAsset()->GetRefSkeleton().GetNum() == GetBoneSpaceTransforms().Num());
	check(GetSkinnedAsset()->GetRefSkeleton().GetNum() == GetNumComponentSpaceTransforms());
	check(GetSkinnedAsset()->GetRefSkeleton().GetNum() == GetBoneVisibilityStates().Num());

	const int32 NumBones = GetBoneSpaceTransforms().Num();

#if DO_GUARD_SLOW
	/** Keep track of which bones have been processed for fast look up */        
	TArray<uint8, TInlineAllocator<256>> BoneProcessed;
	BoneProcessed.AddZeroed(NumBones);
#endif
	// Build in 3 passes.
	// Copy the transform arrays locally (safe even if returned by value)
	const TArray<FTransform> BoneSpace = GetBoneSpaceTransforms();
	const TArray<FTransform>& ComponentSpace = GetEditableComponentSpaceTransforms();

	const FTransform* LocalTransformsData = BoneSpace.GetData();
	FTransform* SpaceBasesData = GetEditableComponentSpaceTransforms().GetData();
	
	/* Old code that was Android Compatible for history record:
	TArray<FTransform> LocalTransforms = GetBoneSpaceTransforms();
	FTransform* LocalTransformsData = LocalTransforms.GetData();
	FTransform* SpaceBasesData = GetEditableComponentSpaceTransforms().GetData();
	*/

	
	GetEditableComponentSpaceTransforms()[0] = GetBoneSpaceTransforms()[0];
#if DO_GUARD_SLOW
	BoneProcessed[0] = 1;
#endif

	for (int32 BoneIndex = 1; BoneIndex < GetBoneSpaceTransforms().Num(); BoneIndex++)
	{
		//FPlatformMisc::Prefetch(SpaceBasesData + BoneIndex);

#if DO_GUARD_SLOW
		// Mark bone as processed
		BoneProcessed[BoneIndex] = 1;
#endif
		// For all bones below the root, final component-space transform is relative transform * component-space transform of parent.
		const int32 ParentIndex = GetSkinnedAsset()->GetRefSkeleton().GetParentIndex(BoneIndex);
		//FPlatformMisc::Prefetch(SpaceBasesData + ParentIndex);

#if DO_GUARD_SLOW
		// Check the precondition that Parents occur before Children in the RequiredBones array.
		checkSlow(BoneProcessed[ParentIndex] == 1);
#endif
		//FTransform::Multiply(SpaceBasesData + BoneIndex, LocalTransformsData + BoneIndex, SpaceBasesData + ParentIndex);

		checkSlow(GetEditableComponentSpaceTransforms()[BoneIndex].IsRotationNormalized());
		checkSlow(!GetEditableComponentSpaceTransforms()[BoneIndex].ContainsNaN());
	}
	bNeedToFlipSpaceBaseBuffers = true;
}



void ULimbMeshComponent::RefreshBoneTransforms(FActorComponentTickFunction* TickFunction)
{

	// Can't do anything without a SkinnedAsset
	if (!GetSkinnedAsset())
	{
		return;
	}

	// Do nothing more if no bones in skeleton.
	if (GetNumComponentSpaceTransforms() == 0)
	{
		return;
	}

	//cout << "Working";
	// We need the mesh space bone transforms now for renderer to get delta from ref pose:
	
	FillComponentSpaceTransforms();

	FinalizeBoneTransform();

	UpdateChildTransforms();
	UpdateBounds();
	MarkRenderTransformDirty();
	MarkRenderDynamicDataDirty();

	bNeedsRefreshTransform = false;


}




void ULimbMeshComponent::CopyPoseFromSkeletalComponent(USkeletalMeshComponent* InComponentToCopy)
{

	TArray<FTransform> LocalTransforms = InComponentToCopy->GetBoneSpaceTransforms();
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	if (LocalTransforms.Num() == BoneSpaceTransforms.Num())
	{
		Exchange(BoneSpaceTransforms, LocalTransforms);

	}
	else
	{
		// The meshes don't match, search bone-by-bone (slow path)

		// first set the localatoms to ref pose from our current mesh
		BoneSpaceTransforms = GetSkinnedAsset()->GetRefSkeleton().GetRefBonePose();

		// Now overwrite any matching bones
		const int32 NumSourceBones = InComponentToCopy->GetSkinnedAsset()->GetRefSkeleton().GetNum();

		for (int32 SourceBoneIndex = 0; SourceBoneIndex < NumSourceBones; ++SourceBoneIndex)
		{
			const FName SourceBoneName = InComponentToCopy->GetBoneName(SourceBoneIndex);
			const int32 TargetBoneIndex = GetBoneIndex(SourceBoneName);

			if (TargetBoneIndex != INDEX_NONE)
			{
				BoneSpaceTransforms[TargetBoneIndex] = LocalTransforms[SourceBoneIndex];
			}
		}

	}

	GetSkeletalMeshAsset()->FillComponentSpaceTransforms(BoneSpaceTransforms, FillComponentSpaceTransformsRequiredBones, GetEditableComponentSpaceTransforms());
	bNeedToFlipSpaceBaseBuffers = true; // Have updated space bases so need to flip
	FlipEditableSpaceBases();

	MarkRefreshTransformDirty();

	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void ULimbMeshComponent::RemoveBodyCollisionsByName(FName BoneName, FName BoneLimb, EPhysBodyOp PhysBodyOption)
{
	// Find appropriate BoneIndex
	int32 BoneIndex = GetBoneIndex(BoneName);

	int32 BoneLimbIndex = GetBoneIndex(BoneLimb);

	if (BoneIndex != INDEX_NONE)
	{
		RemoveBodyCollisions(BoneIndex, BoneLimbIndex, PhysBodyOption);
	}
}

void ULimbMeshComponent::RemoveBodyCollisions(int32 BoneIndex, int32 BoneLimbIndex, EPhysBodyOp PhysBodyOption)
{

	if (!GetSkeletalMeshAsset())
	{
		return;
	}

	if (LeaderPoseComponent.IsValid())
	{
		return;
	}

	// if valid bone index
	if (BoneIndex >= 0 && GetNumBones() > BoneIndex)
	{
		bRequiredBonesUpToDate = false;

		if (PhysBodyOption != PBO_None)
		{
			FName HideBoneName = GetSkeletalMeshAsset()->GetRefSkeleton().GetBoneName(BoneIndex);
			FName HideLimbBoneName = NAME_None;

			if (BoneLimbIndex >= 0 && GetNumBones() > BoneLimbIndex)
			{
				HideLimbBoneName = GetSkeletalMeshAsset()->GetRefSkeleton().GetBoneName(BoneLimbIndex);
			}
			if (PhysBodyOption == PBO_Term)
			{
				TermCollisionsBelow(HideBoneName, HideLimbBoneName);
			}
		}
		else
		{
			UE_LOG(LogSkeletalMesh, Warning, TEXT("PhysBodyOption == None"));
		}
	}
	else
	{
		UE_LOG(LogSkeletalMesh, Warning, TEXT("HideBone[%s]: Invalid Body Index (%d) has entered. This component doesn't contain buffer for the given body."), *GetPathNameSafe(GetSkeletalMeshAsset()), BoneIndex);
	}
}

void ULimbMeshComponent::TermCollisionsBelow(FName ParentBoneName, FName LimbBoneName)
{

	UPhysicsAsset* const PhysicsAsset = GetPhysicsAsset();
	if (PhysicsAsset && GetSkeletalMeshAsset() && Bodies.Num() > 0)
	{
		check(Bodies.Num() == PhysicsAsset->SkeletalBodySetups.Num());

		// Get index of parent bone
		int32 ParentBoneIndex = GetBoneIndex(ParentBoneName);

		int32 LimbBoneIndex = GetBoneIndex(LimbBoneName);

		if (ParentBoneIndex == INDEX_NONE)
		{
			UE_LOG(LogSkeletalMesh, Log, TEXT("TermBodiesBelow: ParentBoneName '%s' is invalid"), *ParentBoneName.ToString());
			return;
		}

		// First terminate any constraints at below this bone
		for (int32 i = 0; i < Constraints.Num(); i++)
		{
			// Get bone index of constraint
			FName JointChildBoneName = Constraints[i]->GetChildBoneName();
			int32 JointBoneIndex = GetBoneIndex(JointChildBoneName);

			// If constraint has bone in mesh, and is either the parent or child of it, term it
			if (JointBoneIndex != INDEX_NONE && (JointChildBoneName == ParentBoneName || GetSkeletalMeshAsset()->GetRefSkeleton().BoneIsChildOf(JointBoneIndex, ParentBoneIndex)))
			{

				if (LimbBoneIndex != INDEX_NONE)
				{
					if (JointChildBoneName != LimbBoneName && !GetSkeletalMeshAsset()->GetRefSkeleton().BoneIsChildOf(JointBoneIndex, LimbBoneIndex))
					{
						Constraints[i]->TermConstraint();
					}
				}
				else
				{
					Constraints[i]->TermConstraint();
				}
			}
		}

		// Then iterate over bodies looking for any which are children of supplied parent
		for (int32 i = 0; i < Bodies.Num(); i++)
		{
			// Get bone index of body
			if (Bodies[i]->IsValidBodyInstance())
			{
				FName BodyName = Bodies[i]->BodySetup->BoneName;
				int32 BodyBoneIndex = GetBoneIndex(BodyName);

				// If body has bone in mesh, and is either the parent or child of it, term it
				if (BodyBoneIndex != INDEX_NONE && (BodyName == ParentBoneName || GetSkeletalMeshAsset()->GetRefSkeleton().BoneIsChildOf(BodyBoneIndex, ParentBoneIndex)))
				{
					if (LimbBoneIndex != INDEX_NONE)
					{
						if (BodyName != LimbBoneName && !GetSkeletalMeshAsset()->GetRefSkeleton().BoneIsChildOf(BodyBoneIndex, LimbBoneIndex))
						{
							Bodies[i]->TermBody();							
						}
					}
					else
					{
						Bodies[i]->TermBody();
					}
				}
			}
		}
		


		//Register the component back to trigger hit events
		if (FPhysScene_Chaos* PhysScene = GetWorld()->GetPhysicsScene())
		{

			PhysScene->RegisterForCollisionEvents(this);
			
		}
		
	}
}
void ULimbMeshComponent::RemovePostProcessAnimInstance()
{
	PostProcessAnimInstance = nullptr;
}


void ULimbMeshComponent::PrintTransforms(const TArray<FTransform>& Transforms)
{
	for (int32 i = 0; i < Transforms.Num(); ++i)
	{
		const FTransform& Transform = Transforms[i];
		PrintTransform(Transform, i);
	}
}
void ULimbMeshComponent::PrintTransform(const FTransform& Transform, int32 i)
{
	FVector Location = Transform.GetLocation(); // Position
	FRotator Rotation = Transform.GetRotation().Rotator(); // Rotation 
	FVector Scale = Transform.GetScale3D(); // Scale

	// Format string
	FString TransformString = FString::Printf(
		TEXT("Transform %d: Location = (X=%.2f, Y=%.2f, Z=%.2f), Rotation = (Pitch=%.2f, Yaw=%.2f, Roll=%.2f), Scale = (X=%.2f, Y=%.2f, Z=%.2f)"), 
		i,
		Location.X, Location.Y, Location.Z,
		Rotation.Pitch, Rotation.Yaw, Rotation.Roll,
		Scale.X, Scale.Y, Scale.Z
	);

	
	UE_LOG(LogTemp, Log, TEXT("%s"), *TransformString);

	// Optionnel : Showscreen message for debug
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TransformString);
	}
}

ULimbMeshComponent::ULimbMeshComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) 
{

	PrimaryComponentTick.bCanEverTick = true;
	bDoubleBufferedComponentSpaceTransforms = 0;
}

void ULimbMeshComponent::ForceGroomLOD(int32 LODIndex)
{
	// Get the owner of this component
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("GroomLODController: No Owner Found!"));
		return;
	}

	// Get ALL GroomComponents attached to the actor
	TArray<UGroomComponent*> GroomComponents;
	Owner->GetComponents<UGroomComponent>(GroomComponents);

	if (GroomComponents.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GroomLODController: No GroomComponents found on %s"), *Owner->GetName());
		return;
	}

	// Apply LOD setting to each GroomComponent
	for (UGroomComponent* GroomComp : GroomComponents)
	{
		if (GroomComp)
		{
			GroomComp->SetForcedLOD(LODIndex);  // Force LOD
		}
	}

}