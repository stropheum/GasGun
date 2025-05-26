// Copyright 2025 Dale "Stropheum" Diaz

#include "GasGunGameMode.h"

#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

AGasGunGameMode::AGasGunGameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	bReplicates = true;
}

void AGasGunGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerControllerList.Add(NewPlayer);
	const FString DebugString = FString::Printf(TEXT("Player Login: %s"), *NewPlayer->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugString);
}

void AGasGunGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGasGunGameMode, PlayerControllerList);
}
