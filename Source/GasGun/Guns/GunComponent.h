// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GunComponent.generated.h"

class APlayerCharacter;
class UFireGunAbility_Base;
class UInputAction;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASGUN_API UGunComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UGunComponent();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(APlayerCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ActivatePrimaryFireAbility();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void DeactivatePrimaryFireAbility();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ActivateSecondaryFireAbility();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void DeactivateSecondaryFireAbility();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetPrimaryFireAbility(TSubclassOf<UFireGunAbility_Base> FireWeaponAbilityClass);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetSecondaryFireAbility(TSubclassOf<UFireGunAbility_Base> FireWeaponAbilityClass);
	
	TTuple<FVector, FRotator> GetProjectileSpawnPositionRotation() const;

	FVector GetMuzzleOffset() const { return MuzzleOffset; }

	UFUNCTION(BlueprintCallable, Category="Player")
	APlayerCharacter* GetOwningPlayer() const;

protected:
	UFUNCTION()
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_FireAbilityHandle();

	UPROPERTY(Replicated, EditDefaultsOnly, Category=Ability, meta=(AllowPrivateAccess=true))
	TSubclassOf<UFireGunAbility_Base> DefaultPrimaryFireAbilityClass;

	UPROPERTY(Replicated, EditDefaultsOnly, Category=Ability, meta=(AllowPrivateAccess=true))
	TSubclassOf<UFireGunAbility_Base> DefaultSecondaryFireAbilityClass;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category=Gameplay, meta=(AllowPrivateAccess=true))
	FVector MuzzleOffset;

	UPROPERTY(Replicated, EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UInputMappingContext> FireMappingContext{};

	UPROPERTY(Replicated, EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> PrimaryFireAction{};

	UPROPERTY(Replicated, EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> SecondaryFireAction{};

	UPROPERTY(ReplicatedUsing = OnRep_FireAbilityHandle)
	FGameplayAbilitySpecHandle PrimaryFireAbilityHandle;

	UPROPERTY(ReplicatedUsing = OnRep_FireAbilityHandle)
	FGameplayAbilitySpecHandle SecondaryFireAbilityHandle;

	UPROPERTY(Replicated)
	TObjectPtr<UFireGunAbility_Base> PrimaryFireGunAbility{};

	UPROPERTY(Replicated)
	TObjectPtr<UFireGunAbility_Base> SecondaryFireGunAbility{};

private:
	TWeakObjectPtr<APlayerCharacter> CharacterWeakPtr{};
};
