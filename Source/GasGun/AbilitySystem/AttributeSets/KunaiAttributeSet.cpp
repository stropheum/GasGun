// Copyright 2025 Dale "Stropheum" Diaz

#include "KunaiAttributeSet.h"


UKunaiAttributeSet::UKunaiAttributeSet()
{
	Damage = 75.f;
}

void UKunaiAttributeSet::PostInitProperties()
{
	Super::PostInitProperties();
	Damage = 75.f;
}
