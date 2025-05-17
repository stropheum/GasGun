// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunAttachmentComponentBase.generated.h"


class UGunComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASGUN_API UGunAttachmentComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UGunAttachmentComponentBase();
	virtual void InitializeAttachment(){};
	void SetOwningGun(UGunComponent* Gun);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category=Attachments, meta=(AllowPrivateAccess=true))
	UGunComponent* OwningGun{};

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
