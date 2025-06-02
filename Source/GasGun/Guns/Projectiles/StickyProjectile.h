// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "StickyProjectile.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
struct FGameplayTag;
class UAbilitySystemComponent;

UCLASS()
class GASGUN_API AStickyProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AStickyProjectile();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Detonate();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category=Niagara, meta=(AllowPrivateAccess=true))
	TObjectPtr<UNiagaraSystem> DetonationNiagaraSystem{};

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category=Audio, meta=(AllowPrivateAccess=true))
	TObjectPtr<USoundBase> DetonationSound{};

	virtual void OnTagChanged(const FGameplayTag Tag, int32 NewCount) override;
	
	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
