// Copyright Epic Games, Inc. All Rights Reserved.

#include "Witcher3GameMode.h"
#include "Witcher3Character.h"
#include "UObject/ConstructorHelpers.h"

AWitcher3GameMode::AWitcher3GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
