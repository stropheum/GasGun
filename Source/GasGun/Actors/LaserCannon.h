// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserCannon.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class GASGUN_API ALaserCannon : public AActor
{
	GENERATED_BODY()

public:
	ALaserCannon();
	
	UFUNCTION(BlueprintCallable)
	void ActivateBeam(bool bActive);

	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* GetNiagaraEffect() const { return NiagaraEffect; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraComponent* NiagaraEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* NiagaraSystemAsset;

private:
	void InitializeNiagaraSystem() const;
	void PerformRaycast() const;

	bool bIsFiring = false;
};
