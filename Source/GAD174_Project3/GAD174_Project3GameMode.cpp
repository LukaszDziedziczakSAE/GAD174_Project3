// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAD174_Project3GameMode.h"
#include "GAD174_Project3Character.h"
#include "UObject/ConstructorHelpers.h"

AGAD174_Project3GameMode::AGAD174_Project3GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
