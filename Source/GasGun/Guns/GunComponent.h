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
	void Fire();

	FVector GetProjectileSpawnLocation() const;

protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta=(AllowPrivateAccess=true))
	TSubclassOf<class UFireGunAbility> FireWeaponAbilityClass;

	UPROPERTY(EditDefaultsOnly, Category=Projectile, meta=(AllowPrivateAccess=true))
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay, meta=(AllowPrivateAccess=true))
	USoundBase* FireSound{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta=(AllowPrivateAccess=true))
	UAnimMontage* FireAnimation{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay, meta=(AllowPrivateAccess=true))
	FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext{};

	UPROPERTY(EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction{};

	UPROPERTY()
	FGameplayAbilitySpecHandle FireAbilityHandle;

	UPROPERTY()
	UFireGunAbility* FireWeaponAbility{};

private:
	TWeakObjectPtr<APlayerCharacter> CharacterWeakPtr{};
};
