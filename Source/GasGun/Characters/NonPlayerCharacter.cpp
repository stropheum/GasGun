// Copyright 2025 Dale "Stropheum" Diaz


#include "NonPlayerCharacter.h"


ANonPlayerCharacter::ANonPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANonPlayerCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANonPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

