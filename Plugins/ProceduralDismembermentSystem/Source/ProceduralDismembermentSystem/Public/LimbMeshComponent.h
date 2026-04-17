// Copyright to Benoit L'hoest 2025

#pragma once


#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "LimbMeshComponent.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (Rendering), meta = (BlueprintSpawnableComponent))
class PROCEDURALDISMEMBERMENTSYSTEM_API ULimbMeshComponent : public USkeletalMeshComponent
{

	GENERATED_BODY()

public:
	FBoneContainer RequiredBonesPos;
	
	UFUNCTION(BlueprintCallable, Category = "Bones Manipulation")
	void SetBoneScaleByName(FName BoneName, FVector InScale3D, EBoneSpaces::Type BoneSpace);
	UFUNCTION(BlueprintCallable, Category = "Bones Manipulation")
	void SetBoneTransformByName(FName BoneName, const FTransform& InTransform, EBoneSpaces::Type BoneSpace);

	UFUNCTION(BlueprintCallable, Category = "Bones Manipulation")
	void CopyPoseFromSkeletalComponent(USkeletalMeshComponent* InComponentToCopy);

	//Constructor and Tick
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	ULimbMeshComponent(const FObjectInitializer& ObjectInitializer);

	//Override bone transform
	void FillComponentSpaceTransforms();
	void MarkRefreshTransformDirty();
	bool IsRunningParallelEvaluation() const { return false; }
	bool ShouldUpdateTransform(bool bLODHasChanged) const override;
	void RefreshBoneTransforms(FActorComponentTickFunction* TickFunction) override;
	

	//Bone Collisions
	UFUNCTION(BlueprintCallable, Category = "Bones Manipulation")
	void RemoveBodyCollisionsByName(FName BoneName, FName BoneLimb, EPhysBodyOp PhysBodyOption);
	void RemoveBodyCollisions(int32 BoneIndex, int32 BoneLimbIndex, EPhysBodyOp PhysBodyOption);
	void TermCollisionsBelow(FName ParentBoneName, FName LimbBoneName);

	//Post Process AnimBP
	UFUNCTION(BlueprintCallable, Category = "Bones Manipulation")
	void RemovePostProcessAnimInstance();


	// utils
	void PrintTransforms(const TArray<FTransform>& Transforms);
	void PrintTransform(const FTransform& Transform, int32 i);
	
	//Groom
	UFUNCTION(BlueprintCallable, Category = "Groom Manipulation")
	void ForceGroomLOD(int32 LODIndex);

private:
	// this is marked if transform has to be updated
	bool bNeedsRefreshTransform;

};
