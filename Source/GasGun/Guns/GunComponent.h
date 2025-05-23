// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GunComponent.generated.h"

class APlayerCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASGUN_API UGunComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UGunComponent();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(APlayerCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ActivateFireAbility();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void DeactivateFireAbility();
	
	TTuple<FVector, FRotator> GetProjectileSpawnPositionRotation() const;

	FVector GetMuzzleOffset() const { return MuzzleOffset; }

	TWeakObjectPtr<APlayerCharacter> GetOwningPlayerWeakPtr() const;

protected:
	UFUNCTION()
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category=Ability, meta=(AllowPrivateAccess=true))
	TSubclassOf<class UFireGunAbility_Base> FireWeaponAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay, meta=(AllowPrivateAccess=true))
	FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess=true))
	class UInputMappingContext* FireMappingContext{};

	UPROPERTY(EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess=true))
	class UInputAction* FireAction{};

	UPROPERTY()
	FGameplayAbilitySpecHandle FireAbilityHandle;

	UPROPERTY()
	UFireGunAbility_Base* FireGunAbility{};

private:
	TWeakObjectPtr<APlayerCharacter> CharacterWeakPtr{};
};
