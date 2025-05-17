// Copyright 2025 Dale "Stropheum" Diaz


#include "GunBeamAttachment.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "GasGun/Characters/CharacterBase.h"
#include "GasGun/Guns/GunComponent.h"


UGunBeamAttachment::UGunBeamAttachment()
{
	PrimaryComponentTick.bCanEverTick = true;
	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
}

void UGunBeamAttachment::InitializeAttachment()
{
	Super::InitializeAttachment();

	InitializeNiagaraComponent();

	check(OwningGun);
	if (!NiagaraSystemAsset) return;
	
	UNiagaraComponent* BeamFX = NewObject<UNiagaraComponent>(GetOwner());
	BeamFX->SetAsset(NiagaraSystemAsset);

	BeamFX->AttachToComponent(OwningGun, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Muzzle"));
	BeamFX->RegisterComponent();
	BeamFX->Activate();

	this->NiagaraEffect = BeamFX;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Spawned Niagara Effect @ %s"), *OwningGun->GetMuzzleOffset().ToString()));
}

void UGunBeamAttachment::BeginPlay()
{
	Super::BeginPlay();
}

void UGunBeamAttachment::InitializeNiagaraComponent() const
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

void UGunBeamAttachment::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PerformRaycast();
}

void UGunBeamAttachment::PerformRaycast() const
{
    const FVector Start = OwningGun->GetComponentLocation();
    const FVector ForwardVector = OwningGun->GetRightVector();
    constexpr float RayLength = 10000.0f;
    const FVector End = Start + (ForwardVector * RayLength);

    FHitResult Hit;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());
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

        if (OtherActor && ImpactComponent)
        {
            constexpr float LaserForce = 12500.0f;
            constexpr float LaserDamage = 1.0f;

            float CosTheta = FVector::DotProduct(ForwardVector, Hit.ImpactNormal);
            float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(CosTheta));
            UE_LOG(LogTemp, Log, TEXT("Hit Angle: %f degrees, CosTheta: %f"), AngleDegrees, CosTheta);

            if (ImpactCharacter)
            {
                // ImpactCharacter->Damage(LaserDamage);
            	//TODO: wire up damaging
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

        	//TODO: This needs to ultimately be handled by the ability itself, we just need to expose beam end and supply it in the ability
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No hit detected! Check cube collision settings."));
    }

    NiagaraEffect->SetVectorParameter(TEXT("Beam End"), BeamEnd);
}