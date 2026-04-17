// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameMode_1.h"
EActionState AGameMode_1::GetCurrentActionState_Implementation()
{
    return CurrentActionState;
}

APawn* AGameMode_1::GetPlayerRef_Implementation()
{
    return PlayerPawnRef;
}

void AGameMode_1::ChangeCurrentActionState_Implementation(EActionState ActionState)
{
    CurrentActionState = ActionState;
}

void AGameMode_1::SetPlayerRef_Implementation(APawn*PawnRef)
{   
    PlayerPawnRef = PawnRef;
}
