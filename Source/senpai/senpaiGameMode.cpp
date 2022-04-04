// Copyright Epic Games, Inc. All Rights Reserved.

#include "senpaiGameMode.h"
#include "senpaiPlayerController.h"
#include "senpaiCharacter.h"
#include "UObject/ConstructorHelpers.h"

AsenpaiGameMode::AsenpaiGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AsenpaiPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}