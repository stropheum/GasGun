// Copyright 2025 Dale "Stropheum" Diaz

#include "GunComponent.h"

#include "AbilitySystemComponent.h"
#include "../Characters/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilitySpec.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GasGun/AbilitySystem/Abilities/FireGunAbility_Base.h"
#include "Net/UnrealNetwork.h"

UGunComponent::UGunComponent()
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = true;
	MuzzleOffset = FVector(56.5f, 14.25f, 11.3f);
	AimingRange = 10000.0f;
	bEnableAimAssist = true;
	GunPositionOffset = FVector(20.0f, 15.0f, 0.0f);
	GunModelCorrection = FRotator(0.0f, -90.0f, 0.0f);
}

void UGunComponent::ActivatePrimaryFireAbility()
{
	if (!CharacterWeakPtr.IsValid() || !CharacterWeakPtr->GetController())
	{
		return;
	}
	
	UAbilitySystemComponent* Asc = CharacterWeakPtr->GetAbilitySystemComponent();
	if (!Asc || !PrimaryFireAbilityHandle.IsValid() || !PrimaryFireGunAbility)
	{
		return;
	}

	FGameplayAbilitySpec* AbilitySpec = Asc->FindAbilitySpecFromHandle(PrimaryFireAbilityHandle);
	if (!AbilitySpec)
	{
		return;
	}

	Asc->TryActivateAbility(PrimaryFireAbilityHandle);
}

void UGunComponent::DeactivatePrimaryFireAbility()
{
	if (!PrimaryFireAbilityHandle.IsValid() || !PrimaryFireGunAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("No FireRifleAbility set"));
		return;
	}

	if (!CharacterWeakPtr.IsValid())
	{
		return;
	}
	
	UAbilitySystemComponent* Asc = CharacterWeakPtr->GetAbilitySystemComponent();
	Asc->CancelAbilityHandle(PrimaryFireAbilityHandle);
}

void UGunComponent::ActivateSecondaryFireAbility()
{
	if (!CharacterWeakPtr.IsValid() || !CharacterWeakPtr->GetController())
	{
		return;
	}
	
	UAbilitySystemComponent* Asc = CharacterWeakPtr->GetAbilitySystemComponent();
	if (!Asc || !SecondaryFireAbilityHandle.IsValid() || !SecondaryFireGunAbility)
	{
		return;
	}

	FGameplayAbilitySpec* AbilitySpec = Asc->FindAbilitySpecFromHandle(SecondaryFireAbilityHandle);
	if (!AbilitySpec)
	{
		return;
	}

	Asc->TryActivateAbility(SecondaryFireAbilityHandle);
}

void UGunComponent::DeactivateSecondaryFireAbility()
{
	if (!SecondaryFireAbilityHandle.IsValid() || !SecondaryFireGunAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("No FireAbility set"));
		return;
	}

	if (!CharacterWeakPtr.IsValid())
	{
		return;
	}

	UAbilitySystemComponent* Asc = CharacterWeakPtr->GetAbilitySystemComponent();
	Asc->CancelAbilityHandle(SecondaryFireAbilityHandle);
}

void UGunComponent::SetPrimaryFireAbility(TSubclassOf<UFireGunAbility_Base> FireWeaponAbilityClass)
{
	if (CharacterWeakPtr->HasAuthority() && FireWeaponAbilityClass && CharacterWeakPtr->GetAbilitySystemComponent())
	{
		if (CharacterWeakPtr->HasAuthority())
		{
			UAbilitySystemComponent* Asc = CharacterWeakPtr->GetAbilitySystemComponent();
			const FGameplayAbilitySpec AbilitySpec(FireWeaponAbilityClass, 1, -1, this);

			if (PrimaryFireAbilityHandle.IsValid())
			{
				Asc->CancelAbilityHandle(PrimaryFireAbilityHandle);
			}
			
			PrimaryFireAbilityHandle = Asc->GiveAbility(AbilitySpec);

			CharacterWeakPtr->ForceNetUpdate();
            
			PrimaryFireGunAbility = Cast<UFireGunAbility_Base>(AbilitySpec.Ability);
		}
	}
}

void UGunComponent::SetSecondaryFireAbility(TSubclassOf<UFireGunAbility_Base> FireWeaponAbilityClass)
{
	if (CharacterWeakPtr->HasAuthority() && FireWeaponAbilityClass && CharacterWeakPtr->GetAbilitySystemComponent())
	{
		if (CharacterWeakPtr->HasAuthority())
		{
			UAbilitySystemComponent* Asc = CharacterWeakPtr->GetAbilitySystemComponent();
			const FGameplayAbilitySpec AbilitySpec(FireWeaponAbilityClass, 1, -1, this);

			if (SecondaryFireAbilityHandle.IsValid())
			{
				Asc->CancelAbilityHandle(SecondaryFireAbilityHandle);
			}
			
			SecondaryFireAbilityHandle = Asc->GiveAbility(AbilitySpec);

			CharacterWeakPtr->ForceNetUpdate();
            
			SecondaryFireGunAbility = Cast<UFireGunAbility_Base>(AbilitySpec.Ability);
		}
	}
}

TTuple<FVector, FRotator> UGunComponent::GetProjectileSpawnPositionRotation() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	const FVector SpawnPosition = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	return {SpawnPosition, SpawnRotation};
}

bool UGunComponent::AttachWeapon(APlayerCharacter* TargetCharacter)
{
	CharacterWeakPtr = TargetCharacter;

	if (CharacterWeakPtr == nullptr || CharacterWeakPtr->GetInstanceComponents().FindItemByClass<UGunComponent>())
	{
		return false;
	}

	// Use SnapToTarget to get the initial transform correct
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(CharacterWeakPtr->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	SetPrimaryFireAbility(DefaultPrimaryFireAbilityClass);

	if (const APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController()))
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(CharacterWeakPtr))
		{
			PlayerCharacter->SetGun(this);
		}

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(PrimaryFireAction, ETriggerEvent::Triggered, this, &UGunComponent::ActivatePrimaryFireAbility);
			EnhancedInputComponent->BindAction(PrimaryFireAction, ETriggerEvent::Completed, this, &UGunComponent::DeactivatePrimaryFireAbility);
			EnhancedInputComponent->BindAction(SecondaryFireAction, ETriggerEvent::Triggered, this, &UGunComponent::ActivateSecondaryFireAbility);
			EnhancedInputComponent->BindAction(SecondaryFireAction, ETriggerEvent::Completed, this, &UGunComponent::DeactivateSecondaryFireAbility);
		}
	}

	return true;
}

APlayerCharacter* UGunComponent::GetOwningPlayer() const
{
	return CharacterWeakPtr.Get();
}

void UGunComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGunComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CharacterWeakPtr.IsValid())
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(FireMappingContext);
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UGunComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGunComponent, DefaultPrimaryFireAbilityClass);
	DOREPLIFETIME(UGunComponent, DefaultSecondaryFireAbilityClass);
	DOREPLIFETIME(UGunComponent, MuzzleOffset);
	DOREPLIFETIME(UGunComponent, FireMappingContext);
	DOREPLIFETIME(UGunComponent, PrimaryFireAction);
	DOREPLIFETIME(UGunComponent, SecondaryFireAction);
	DOREPLIFETIME(UGunComponent, PrimaryFireGunAbility);
	DOREPLIFETIME(UGunComponent, SecondaryFireGunAbility);
	DOREPLIFETIME(UGunComponent, AimingRange);
	DOREPLIFETIME(UGunComponent, bEnableAimAssist);
	DOREPLIFETIME(UGunComponent, GunPositionOffset);
	DOREPLIFETIME(UGunComponent, GunModelCorrection);
	DOREPLIFETIME_CONDITION_NOTIFY(UGunComponent, PrimaryFireAbilityHandle, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGunComponent, SecondaryFireAbilityHandle, COND_None, REPNOTIFY_Always);
}

void UGunComponent::OnRep_FireAbilityHandle()
{
	if (CharacterWeakPtr.IsValid() && CharacterWeakPtr->GetAbilitySystemComponent())
	{
		FGameplayAbilitySpec* AbilitySpec = CharacterWeakPtr->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(PrimaryFireAbilityHandle);
		if (AbilitySpec)
		{
			PrimaryFireGunAbility = Cast<UFireGunAbility_Base>(AbilitySpec->Ability);
			UE_LOG(LogTemp, Warning, TEXT("OnRep_FireAbilityHandle: Set FireGunAbility %s"), 
				PrimaryFireGunAbility ? TEXT("Successfully") : TEXT("Failed"));
		}
	}
}

void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bEnableAimAssist && CharacterWeakPtr.IsValid())
	{
		UpdateGunAimingWithIK();
		const TTuple<FVector, FRotator> SpawnPositionRotation = GetProjectileSpawnPositionRotation();
		DrawDebugLine(GetWorld(), SpawnPositionRotation.Key, GetCameraAimHitLocation(), FColor::Red, false, 0.0f, 0.0f, 2.0f);
	}
}

void UGunComponent::UpdateGunAimingWithIK()
{
	if (!bEnableAimAssist || !CharacterWeakPtr.IsValid())
	{
		return;
	}

	USkeletalMeshComponent* CharacterMesh = CharacterWeakPtr->GetMesh1P();
	if (!CharacterMesh)
	{
		return;
	}

	FVector HitLocation = GetCameraAimHitLocation();
	FTransform AdjustedTransform = CalculateAdjustedGunTransform(HitLocation);

	SetWorldTransform(AdjustedTransform);
}

FVector UGunComponent::GetCameraAimHitLocation() const
{
	if (!CharacterWeakPtr.IsValid())
	{
		return FVector::ZeroVector;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController());
	if (!PlayerController || !PlayerController->PlayerCameraManager)
	{
		return FVector::ZeroVector;
	}

	FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	FVector CameraForward = CameraRotation.Vector();

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (CameraForward * AimingRange);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(CharacterWeakPtr.Get());
	QueryParams.bTraceComplex = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	return bHit ? HitResult.Location : TraceEnd;
}

FTransform UGunComponent::CalculateAdjustedGunTransform(const FVector& TargetLocation)
{
	USkeletalMeshComponent* CharacterMesh = CharacterWeakPtr->GetMesh1P();
	if (!CharacterMesh)
	{
		return GetComponentTransform();
	}

	const APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController());
	if (!PlayerController || !PlayerController->PlayerCameraManager)
	{
		return GetComponentTransform();
	}

	FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

	FTransform GripSocketTransform = CharacterMesh->GetSocketTransform(FName("GripPoint"));

	FVector CameraForward = CameraRotation.Vector();
	FVector CameraRight = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Y);
	FVector CameraUp = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Z);

	FVector OffsetLocation = GripSocketTransform.GetLocation();
	OffsetLocation += CameraForward * GunPositionOffset.X; // Forward
	OffsetLocation += CameraRight * GunPositionOffset.Y;   // Right
	OffsetLocation += CameraUp * GunPositionOffset.Z;      // Up

	FVector AimDirection = (TargetLocation - OffsetLocation).GetSafeNormal();
	FRotator AimRotation = AimDirection.Rotation();

	FQuat CorrectionQuat = GunModelCorrection.Quaternion();
	FQuat AimQuat = AimRotation.Quaternion();

	FQuat FinalRotation = AimQuat * CorrectionQuat;

	FTransform AdjustedTransform;
	AdjustedTransform.SetLocation(OffsetLocation);
	AdjustedTransform.SetRotation(FinalRotation);
	AdjustedTransform.SetScale3D(GripSocketTransform.GetScale3D());

	return AdjustedTransform;
}
