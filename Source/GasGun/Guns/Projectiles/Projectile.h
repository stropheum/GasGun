// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

struct FGameplayTag;
class UAbilitySystemComponent;
class UGunComponent;
class USphereComponent;
class UProjectileMovementComponent;

USTRUCT(BlueprintType)
struct FFlechetteProjectileConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlechetteProjectileConfig" )
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlechetteProjectileConfig")
	int Count;
	
};

UCLASS(config=Game)
class AProjectile : public AActor
{
	GENERATED_BODY()

public:
	
	AProjectile();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	UGunComponent* GetOwningGun() const;

	UFUNCTION()
	void SetOwningGun(UGunComponent* Gun);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Projectile)
	void OnSetOwningGunCalled(UGunComponent* Gun);

	UFUNCTION(BlueprintCallable, Category=Projectile)
	void RegisterOwnerTagListener(UAbilitySystemComponent* Asc, FGameplayTag TagToRegister);

	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	UFUNCTION()
	virtual void OnTagChanged(const FGameplayTag Tag, int32 NewCount);
	
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	TObjectPtr<USphereComponent> CollisionComp{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement{};

	UPROPERTY()
	TObjectPtr<UGunComponent> OwningGun{};
};

