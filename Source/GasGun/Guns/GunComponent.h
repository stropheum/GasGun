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

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetFireAbility(TSubclassOf<class UFireGunAbility_Base> FireWeaponAbilityClass);
	
	TTuple<FVector, FRotator> GetProjectileSpawnPositionRotation() const;

	FVector GetMuzzleOffset() const { return MuzzleOffset; }

	TWeakObjectPtr<APlayerCharacter> GetOwningPlayerWeakPtr() const;

protected:
	UFUNCTION()
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_FireAbilityHandle();

	UPROPERTY(Replicated, EditDefaultsOnly, Category=Ability, meta=(AllowPrivateAccess=true))
	TSubclassOf<class UFireGunAbility_Base> DefaultFireWeaponAbilityClass;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category=Gameplay, meta=(AllowPrivateAccess=true))
	FVector MuzzleOffset;

	UPROPERTY(Replicated, EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UInputMappingContext> FireMappingContext{};

	UPROPERTY(Replicated, EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UInputAction> FireAction{};

	UPROPERTY(ReplicatedUsing = OnRep_FireAbilityHandle)
	FGameplayAbilitySpecHandle FireAbilityHandle;

	UPROPERTY()
	TObjectPtr<UFireGunAbility_Base> FireGunAbility{};

private:
	TWeakObjectPtr<APlayerCharacter> CharacterWeakPtr{};
};
