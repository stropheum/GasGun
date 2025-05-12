// Copyright 2025 Dale "Stropheum" Diaz

#include "GasGunGameMode.h"
#include "../Characters/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGasGunGameMode::AGasGunGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
