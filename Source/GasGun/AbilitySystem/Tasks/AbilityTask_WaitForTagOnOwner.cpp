// Copyright 2025 Dale "Stropheum" Diaz


#include "AbilityTask_WaitForTagOnOwner.h"

#include "AbilitySystemComponent.h"

UAbilityTask_WaitForTagOnOwner* UAbilityTask_WaitForTagOnOwner::WaitForTagOnOwner(UGameplayAbility* OwningAbility, UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag QueryTag) {
	UAbilityTask_WaitForTagOnOwner* Task = NewAbilityTask<UAbilityTask_WaitForTagOnOwner>(OwningAbility);
	Task->Asc = AbilitySystemComponent;
	Task->QueryTag = QueryTag;
	return Task;
}

void UAbilityTask_WaitForTagOnOwner::Activate()
{
	Super::Activate();

	AbilitySystemComponent
		->RegisterGameplayTagEvent(QueryTag, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &UAbilityTask_WaitForTagOnOwner::OnTagChanged);
}

void UAbilityTask_WaitForTagOnOwner::OnTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Tag %s changed to %d"), *Tag.ToString(), NewCount));
}
