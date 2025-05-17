// Copyright 2025 Dale "Stropheum" Diaz


#include "GunAttachmentComponentBase.h"


UGunAttachmentComponentBase::UGunAttachmentComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGunAttachmentComponentBase::SetOwningGun(UGunComponent* Gun)
{
	OwningGun = Gun;
}

void UGunAttachmentComponentBase::BeginPlay()
{
	Super::BeginPlay();
}

void UGunAttachmentComponentBase::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}