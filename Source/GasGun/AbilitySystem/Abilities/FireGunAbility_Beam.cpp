// Copyright 2025 Dale "Stropheum" Diaz


#include "FireGunAbility_Beam.h"

#include "NiagaraComponent.h"
#include "GasGun/Characters/PlayerCharacter.h"
#include "GasGun/Guns/GunComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GasGun/AbilitySystem/Tasks/AbilityTask_OnTickEvent.h"

void UFireGunAbility_Beam::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwningActor = GetAvatarActorFromActorInfo();
	if (!OwningActor || !NiagaraSystemAsset)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	APlayerCharacter* OwnerPlayerCharacter = Cast<APlayerCharacter>(OwningActor);
	check(OwnerPlayerCharacter);
	OwningGun = OwnerPlayerCharacter->GetGun();

	NiagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraSystemAsset,
		OwningGun,
		FName("Muzzle"),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true);

	UAbilityTask_OnTickEvent* TickTask = UAbilityTask_OnTickEvent::OnTickEvent(this, FName("BeamTickTask"));
	if (TickTask)
	{
		TickTask->TickEventReceived.AddDynamic(this, &UFireGunAbility_Beam::OnBeamTick);
		TickTask->ReadyForActivation();
	}

	OwnerPlayerCharacter->GetAbilitySystemComponent()->AddGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Gun.Beam.Active"));
	// OwnerPlayerCharacter->GetAbilitySystemComponent()->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Gun.Beam.Active"), FGameplayCueParameters());
}

void UFireGunAbility_Beam::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UFireGunAbility_Beam::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	APlayerCharacter* OwnerPlayerCharacter = Cast<APlayerCharacter>(OwningActor);
	check(OwnerPlayerCharacter);
	OwnerPlayerCharacter->GetAbilitySystemComponent()->RemoveGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Gun.Beam.Active"));
	
	if (NiagaraEffect)
	{
		NiagaraEffect->Deactivate();
		NiagaraEffect = nullptr;
	}
	OwningActor = nullptr;
	OwningGun = nullptr;
}

bool UFireGunAbility_Beam::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UFireGunAbility_Beam::InitializeNiagaraSystem()
{
	if (NiagaraEffect && NiagaraSystemAsset)
	{
		NiagaraEffect->SetAsset(NiagaraSystemAsset);
		NiagaraEffect->SetRelativeLocation(FVector::ZeroVector);
		NiagaraEffect->Deactivate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NiagaraSystemAsset or NiagaraEffect is null!"));
	}
}

void UFireGunAbility_Beam::SetBeamActive(bool BeamIsActive)
{
	if (NiagaraEffect == nullptr /*|| NiagaraEffect->IsActive() == BeamIsActive */) { return; }
	
	NiagaraEffect->SetActive(BeamIsActive);
}

void UFireGunAbility_Beam::PerformRaycast() const
{
	check(OwningGun);
	const FVector Start =  OwningGun->GetSocketLocation(FName("Muzzle"));
    const FVector ForwardVector = OwningGun->GetRightVector();
    constexpr float RayLength = 10000.0f;
    const FVector End = Start + (ForwardVector * RayLength);

    FHitResult Hit;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwningActor);
    if (const APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn())
    {
        QueryParams.AddIgnoredActor(PlayerPawn);
    }
	
    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        Start,
        End,
        ECC_GameTraceChannel1,
        QueryParams
    );

    const FVector BeamEnd = bHit ? Hit.ImpactPoint : End;

    if (bHit)
    {
        AActor* OtherActor = Hit.GetActor();
        UPrimitiveComponent* ImpactComponent = Hit.GetComponent();
        ACharacterBase* ImpactCharacter = Cast<ACharacterBase>(OtherActor);
        TWeakObjectPtr<APlayerCharacter> OwningPlayer = OwningGun->GetOwningPlayerWeakPtr();
    	check(OwningPlayer.IsValid());

        if (OtherActor && OtherActor != OwningPlayer.Get() && ImpactComponent)
        {
            constexpr float LaserForce = 12500.0f;
            constexpr float LaserDamage = 1.0f;

            float CosTheta = FVector::DotProduct(ForwardVector, Hit.ImpactNormal);
            float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(CosTheta));
            UE_LOG(LogTemp, Log, TEXT("Hit Angle: %f degrees, CosTheta: %f"), AngleDegrees, CosTheta);

            if (ImpactCharacter)
            {
                // ImpactCharacter->Damage(LaserDamage);
            }

            if (ImpactComponent->IsSimulatingPhysics())
            {
                FVector Impulse = ForwardVector * LaserForce * FMath::Abs(CosTheta);
                ImpactComponent->AddImpulseAtLocation(Impulse, Hit.ImpactPoint);
            }
            // else if (ImpactCharacter && ImpactCharacter->IsDead())
            // {
            //     FVector Impulse = ForwardVector * LaserForce * FMath::Abs(CosTheta);
            //     ImpactCharacter->GetMesh()->AddImpulseAtLocation(Impulse, Hit.ImpactPoint);
            // }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No hit detected! Check cube collision settings."));
    }

    NiagaraEffect->SetVectorParameter(TEXT("Beam End"), BeamEnd);
}

void UFireGunAbility_Beam::OnBeamTick(float DeltaTime)
{
	PerformRaycast();
}
