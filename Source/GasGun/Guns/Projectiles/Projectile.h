// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GasGun/AbilitySystem/ProjectileDirectDamageEffect.h"
#include "Projectile.generated.h"

class UProjectileAttributeSet;
class UGameplayEffect;
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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	virtual void OnTagChanged(const FGameplayTag Tag, int32 NewCount);

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Projectile", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UProjectileAttributeSet> AttributeSet{};
	
	UPROPERTY(Replicated, VisibleDefaultsOnly, Category=Projectile)
	TObjectPtr<USphereComponent> CollisionComp{};

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement{};

	UPROPERTY(Replicated)
	TObjectPtr<UGunComponent> OwningGun{};

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<UGameplayEffect> DamageEffectClass = UProjectileDirectDamageEffect::StaticClass();

	const float Mass = 10.f;
};

