// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UnholyGameMode.h"
#include "UnholyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnholyGameMode::AUnholyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		
		
	}
	
}
