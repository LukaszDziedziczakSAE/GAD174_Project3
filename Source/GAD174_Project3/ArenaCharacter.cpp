// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ArenaGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AArenaCharacter::AArenaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Voice = CreateDefaultSubobject<UAudioComponent>(TEXT("Voice Audio"));
	Voice->SetupAttachment(GetRootComponent());

	Footstep = CreateDefaultSubobject<UAudioComponent>(TEXT("Footstep Audio"));
	Footstep->SetupAttachment(GetRootComponent());

	Health = 1;
}

// Called when the game starts or when spawned
void AArenaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Set Health and stamina to Max
	Health = MaxHealth;
	Stamina = MaxStamina;

	// Spawn Weapon
	if (WeaponClass != nullptr)
	{
		SpawnWeapon(WeaponClass);
	}

	// Record Walking speed
	if (GetCharacterMovement() != nullptr)
	{
		WalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
		//UE_LOG(LogTemp, Warning, TEXT("Walking Speed = %f"), WalkingSpeed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get CharacterMovement"));
	}

	GameMode = Cast<AArenaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void AArenaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Running && !Attacking && !Blocking && Stamina < MaxStamina)
	{
		Stamina += StaminaRecovery * DeltaTime;
	}

	else if (Running)
	{
		if (Stamina > 0)
		{
			Stamina -= StaminaRunningCost * DeltaTime;
		}
		else
		{
			RunningStop();
		}
	}

	else if (Blocking)
	{
		if (Stamina > 0)
		{
			Stamina -= Weapon->GetStaminaBlockingCost() * DeltaTime;
		}
		else
		{
			BlockStop();
		}
	}

	if (Stamina < 0) Stamina = 0;
}

// Called to bind functionality to input
void AArenaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AArenaCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Block"), EInputEvent::IE_Pressed, this, &AArenaCharacter::BlockStart);
	PlayerInputComponent->BindAction(TEXT("Block"), EInputEvent::IE_Released, this, &AArenaCharacter::BlockStop);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AArenaCharacter::RunningStart);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AArenaCharacter::RunningStop);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AArenaCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AArenaCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AArenaCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AArenaCharacter::LookRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AArenaCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AArenaCharacter::LookRightRate);
}

bool AArenaCharacter::IsDead() const
{
	return Health <= 0;
}

bool AArenaCharacter::GetBlockingPressed()
{
	return Blocking;
}

bool AArenaCharacter::GetAttacking()
{
	return Attacking;
}

void AArenaCharacter::AttackComplete()
{
	Attacking = false;
	Weapon->AttackReset();
	//UE_LOG(LogTemp, Warning, TEXT("Attack complete"));
}

void AArenaCharacter::SpawnWeapon(TSubclassOf<AArenaWeapon> weaponClass)
{
	if (Weapon != nullptr) Weapon->Destroy();
	if (Shield != nullptr) Shield->Destroy();

	Weapon = GetWorld()->SpawnActor<AArenaWeapon>(weaponClass);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Weapon->SetOwner(this);

	if (Weapon->HasShield())
	{
		Shield = GetWorld()->SpawnActor<AArenaShield>(Weapon->GetShieldClass());
		Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ShieldSocket"));
		Shield->SetOwner(this);
	}
}

AArenaWeapon* AArenaCharacter::GetWeapon()
{
	return Weapon;
}

AArenaShield* AArenaCharacter::GetShield()
{
	return Shield;
}

bool AArenaCharacter::IsWeaponOneHanded()
{
	if (Weapon == nullptr) return false;
	return Weapon->IsOneHanded();
}

void AArenaCharacter::ApplyDamage(float Amount)
{
	UE_LOG(LogTemp, Log, TEXT("%s took %f damage"), *GetName(), Amount);
	Health -= Amount;

	if (Health > 0)
	{
		PlayImpact();
		if (GruntingGotHitSound != nullptr)
		{
			Voice->SetSound(GruntingGotHitSound);
			Voice->Play();
		}
	}
	else // Character is dead
	{
		if (DeathScreamSound != nullptr)
		{
			Voice->SetSound(DeathScreamSound);
			Voice->Play();
		}

		//UCapsuleComponent* capsule = GetCapsuleComponent();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (OnDeath.IsBound()) OnDeath.Broadcast();
	}

	Attacking = false;
}

void AArenaCharacter::PlayImpact()
{
	if (IsWeaponOneHanded())
	{
		if (Blocking)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(OneHandedBlockingImpactMontage);
		}
		else
		{
			GetMesh()->GetAnimInstance()->Montage_Play(OneHandedImpactMontage);
		}
	}
	else
	{
		if (Blocking)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(TwoHandedBlockingImpactMontage);
		}
		else
		{
			GetMesh()->GetAnimInstance()->Montage_Play(TwoHandedImpactMontage);
		}
	}
}

void AArenaCharacter::IsBlockingStart()
{
	IsBlocking = true;
	UE_LOG(LogTemp, Log, TEXT("%s started blocking"), *GetName());

}

void AArenaCharacter::IsBlockingStop()
{
	IsBlocking = false;
	UE_LOG(LogTemp, Log, TEXT("%s stopped blocking"), *GetName());
}

void AArenaCharacter::LeftFootstep()
{
	if (VFX_Footstep != nullptr)
	{
		if (GetMesh()->GetBoneLocation("ball_l").Equals(FVector::Zero()))
		{
			UE_LOG(LogTemp, Error, TEXT("%s unable to find bone ball_l"), *GetName());
		}
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFX_Footstep, GetMesh()->GetBoneLocation("ball_l"));
	}

	if (FootstepSound != nullptr)
	{
		Footstep->SetSound(FootstepSound);
		Footstep->Play();
	}
}

void AArenaCharacter::RightFootstep()
{
	if (VFX_Footstep != nullptr)
	{
		if (GetMesh()->GetBoneLocation("ball_r").Equals(FVector::Zero()))
		{
			UE_LOG(LogTemp, Error, TEXT("%s unable to find bone ball_r"), *GetName());
		}
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFX_Footstep, GetMesh()->GetBoneLocation("ball_r"));
	}

	if (FootstepSound != nullptr)
	{
		Footstep->SetSound(FootstepSound);
		Footstep->Play();
	}
}

bool AArenaCharacter::GetRunning()
{
	return Running;
}

void AArenaCharacter::Attack()
{
	if (!GameMode->MatchStarted()) return;
	if (Attacking || Blocking) return;

	if (!HasEnoughStamina(Weapon->GetStaminaAttackCost())) return;

	Stamina -= Weapon->GetStaminaAttackCost();
	Attacking = true;

	if (IsWeaponOneHanded())
	{
		if (GetMesh()->GetAnimInstance()->Montage_Play(OneHandedAttackMontage) == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to play montage"));
		};
	}
	else
	{
		if (GetMesh()->GetAnimInstance()->Montage_Play(TwoHandedAttackMontage) == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to play montage"));
		};
	}

	if (GruntingAttackSound != nullptr)
	{
		Voice->SetSound(GruntingAttackSound);
		Voice->Play();
	}
	
	UE_LOG(LogTemp, Log, TEXT("%s attacked"), *GetName());
}

void AArenaCharacter::BlockStart()
{
	if (!GameMode->MatchStarted()) return;
	if (!HasEnoughStamina(Weapon->GetStaminaBlockingStartCost())) return;
	//UE_LOG(LogTemp, Warning, TEXT("Block started"));
	Blocking = true;
	Stamina -= Weapon->GetStaminaBlockingStartCost();
}

void AArenaCharacter::BlockStop()
{
	//if (Attacking || Blocking) return;
	//UE_LOG(LogTemp, Warning, TEXT("Block ended"));
	Blocking = false;
}

void AArenaCharacter::RunningStart()
{
	if (!GameMode->MatchStarted()) return;
	Running = true;
	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	UE_LOG(LogTemp, Warning, TEXT("Set movement to %f"), GetCharacterMovement()->MaxWalkSpeed);
}

void AArenaCharacter::RunningStop()
{
	Running = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	UE_LOG(LogTemp, Warning, TEXT("Set movement to %f"), GetCharacterMovement()->MaxWalkSpeed);
}

float AArenaCharacter::GetHealthPercentage()
{
	return Health/MaxHealth;
}

float AArenaCharacter::GetStaminaPercentage()
{
	return Stamina/MaxStamina;
}

void AArenaCharacter::ResetCharacter()
{
	Health = MaxHealth;
	Stamina = MaxStamina;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AArenaCharacter::SetRunning(bool isRunning)
{
	Running = isRunning;

	if (Running)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	}
}

float AArenaCharacter::GetStaminaRecovery()
{
	return StaminaRecovery;
}

float AArenaCharacter::GetStaminaRunningCost()
{
	return StaminaRunningCost;
}

bool AArenaCharacter::HasEnoughStamina(float StaminaCost)
{
	return Stamina >= StaminaCost;
}

void AArenaCharacter::TakeStamina(float Amount)
{
	Stamina -= Amount;
	if (Stamina < 0) Stamina = 0;
}

void AArenaCharacter::MoveForward(float AxisValue)
{
	if (!GameMode->MatchStarted() || IsDead()) return;
	if (Attacking || Blocking) return;
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AArenaCharacter::MoveRight(float AxisValue)
{
	if (!GameMode->MatchStarted() || IsDead()) return;
	if (Attacking || Blocking) return;
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AArenaCharacter::LookUp(float AxisValue)
{
	if (!GameMode->MatchStarted() || IsDead()) return;
	AddControllerPitchInput(AxisValue);
}

void AArenaCharacter::LookRight(float AxisValue)
{
	if (!GameMode->MatchStarted() || IsDead()) return;
	AddControllerYawInput(AxisValue);
}

void AArenaCharacter::LookUpRate(float AxisValue)
{
	if (!GameMode->MatchStarted() || IsDead()) return;
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AArenaCharacter::LookRightRate(float AxisValue)
{
	if (!GameMode->MatchStarted() || IsDead()) return;
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

