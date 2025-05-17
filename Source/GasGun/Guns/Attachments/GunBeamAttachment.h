// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "GunAttachmentComponentBase.h"
#include "GunBeamAttachment.generated.h"


class UNiagaraSystem;
class UNiagaraComponent;
class UGunComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASGUN_API UGunBeamAttachment : public UGunAttachmentComponentBase
{
	GENERATED_BODY()

public:
	UGunBeamAttachment();

	virtual void InitializeAttachment() override;

protected:
	virtual void BeginPlay() override;

	void InitializeNiagaraComponent() const;

	void PerformRaycast() const;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category=Niagara, meta=(AllowPrivateAccess=true))
	UNiagaraSystem* NiagaraSystemAsset{};

	UPROPERTY()
	UNiagaraComponent* NiagaraEffect{};
};
