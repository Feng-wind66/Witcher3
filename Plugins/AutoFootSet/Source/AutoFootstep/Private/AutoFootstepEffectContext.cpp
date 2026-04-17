// Copyright 2023-2024 Metaseven. All Rights Reserved.

#include "AutoFootstepEffectContext.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundBase.h"

void UAutoFootstepEffectContext::PlayEffectBySurfaceType(const UObject* WorldContextObject, const EPhysicalSurface SurfaceType, const FVector& Location, const FRotator& Rotation, const FAutoFootstepNiagaraParams& NiagaraParams, const FAutoFootstepSoundParams& SoundParams)
{
	if (const FAutoFootstepEffect* Effect = EffectsBySurfaceType.Find(SurfaceType))
	{
		TArray<FSoftObjectPath> EffectPaths;

		if (!Effect->Niagara.IsValid() && !Effect->Niagara.IsNull())
		{
			EffectPaths.Emplace(Effect->Niagara.ToSoftObjectPath());
		}

		if (!Effect->Sound.IsValid() && !Effect->Sound.IsNull())
		{
			EffectPaths.Emplace(Effect->Sound.ToSoftObjectPath());
		}

		if (EffectPaths.IsEmpty())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldContextObject, Effect->Niagara.Get(), Location + NiagaraParams.LocationOffset, Rotation + NiagaraParams.RotationOffset, NiagaraParams.Scale, true, true, NiagaraParams.PoolingMethod);
			UGameplayStatics::PlaySoundAtLocation(WorldContextObject, Effect->Sound.Get(), Location + SoundParams.LocationOffset, Rotation + SoundParams.RotationOffset, SoundParams.VolumeMultiplier, SoundParams.PitchMultiplier);
			return;
		}
		TSoftObjectPtr<UNiagaraSystem> SafeNiagaraPtr = Effect->Niagara;
		TSoftObjectPtr<USoundBase> SafeSoundPtr = Effect->Sound;
		TWeakObjectPtr<const UObject> WeakWorldContext = WorldContextObject;
		UAssetManager::GetStreamableManager().RequestAsyncLoad(EffectPaths, [WeakWorldContext, SafeNiagaraPtr, SafeSoundPtr, Location, Rotation, NiagaraParams, SoundParams]()
		{
			const UObject* SafeContext = WeakWorldContext.Get();
			if (!IsValid(SafeContext))
			{
				return;
			}
			if (UNiagaraSystem* NiagaraAsset = SafeNiagaraPtr.Get())
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(SafeContext, NiagaraAsset, Location + NiagaraParams.LocationOffset, Rotation + NiagaraParams.RotationOffset, NiagaraParams.Scale, true, true, NiagaraParams.PoolingMethod);
			}
			if (USoundBase* SoundAsset = SafeSoundPtr.Get())
			{
				UGameplayStatics::PlaySoundAtLocation(SafeContext, SoundAsset, Location + SoundParams.LocationOffset, Rotation + SoundParams.RotationOffset, SoundParams.VolumeMultiplier, SoundParams.PitchMultiplier);
			}
		});
	}
}

#if WITH_EDITOR
void UAutoFootstepEffectContext::AddAllSurfaceTypeElements()
{
	for (int32 SurfaceType = SurfaceType_Default; SurfaceType < SurfaceType_Max; ++SurfaceType)
	{
		if (StaticEnum<EPhysicalSurface>()->HasMetaData(TEXT("Hidden"), SurfaceType))
		{
			continue;
		}

		if (EffectsBySurfaceType.Contains(StaticCast<EPhysicalSurface>(SurfaceType)))
		{
			continue;
		}

		EffectsBySurfaceType.Add(StaticCast<EPhysicalSurface>(SurfaceType));
	}
}
#endif
