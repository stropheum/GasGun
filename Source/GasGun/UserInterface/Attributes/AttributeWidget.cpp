// Copyright 2025 Dale "Stropheum" Diaz


#include "AttributeWidget.h"

#include "AbilitySystemComponent.h"
#include "../../AbilitySystem/AttributeSets/AttributeSetBase.h"
#include "CoreMinimal.h"
#include "GasGun/AbilitySystem/AttributeSets/CharacterBaseAttributeSet.h"

void UAttributeWidget::BindToAttributeSet(UCharacterBaseAttributeSet* AttributeSet)
{
	AttributeSetContext = AttributeSet;
	UAbilitySystemComponent* Asc = AttributeSet->GetOwningAbilitySystemComponent();

	HealthPercent = AttributeSetContext->GetHealthPercent();
	ShieldPercent = AttributeSetContext->GetShieldPercent();

	Asc->GetGameplayAttributeValueChangeDelegate(AttributeSetContext->GetHealthAttribute())
	   .AddUObject(this, &UAttributeWidget::OnHealthChangeCallback);

	Asc->GetGameplayAttributeValueChangeDelegate(AttributeSetContext->GetShieldAttribute())
		.AddUObject(this, &UAttributeWidget::OnShieldChangeCallback);
}

void UAttributeWidget::OnHealthChangeCallback(const FOnAttributeChangeData& Data)
{
	HealthPercent = AttributeSetContext->GetHealthPercent();
	if (HealthPercent <= 0.f)
	{
		SetVisibility(ESlateVisibility::Hidden);
		SetIsEnabled(false);
	}
}

void UAttributeWidget::OnShieldChangeCallback(const FOnAttributeChangeData& Data)
{
	ShieldPercent = AttributeSetContext->GetShieldPercent();
}