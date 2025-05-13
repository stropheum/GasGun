// Copyright 2025 Dale "Stropheum" Diaz

#include "GasGunGameMode.h"
#include "UObject/ConstructorHelpers.h"

AGasGunGameMode::AGasGunGameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
