// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeWidget.generated.h"

struct FOnAttributeChangeData;
class UCharacterBaseAttributeSet;
/**
 * 
 */
UCLASS()
class GASGUN_API UAttributeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void BindToAttributeSet(UCharacterBaseAttributeSet* AttributeSet);

protected:
	void OnHealthChangeCallback(const FOnAttributeChangeData& Data);

	void OnShieldChangeCallback(const FOnAttributeChangeData& Data);

	UPROPERTY(BlueprintReadOnly)
	float HealthPercent;

	UPROPERTY(BlueprintReadOnly)
	float ShieldPercent;

	UPROPERTY()
	TObjectPtr<UCharacterBaseAttributeSet> AttributeSetContext;
};
