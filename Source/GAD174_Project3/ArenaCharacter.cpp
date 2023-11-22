// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AArenaCharacter::AArenaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Audio = CreateAbstractDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AArenaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Set Health to Max
	Health = MaxHealth;

	// Spawn Weapon
	if (WeaponClass != nullptr)
	{
		SpawnWeapon(WeaponClass);
	}

	// Record Walking speed
	if (GetCharacterMovement() != nullptr)
	{
		WalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Walking Speed = %f"), WalkingSpeed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get CharacterMovement"));
	}
}

// Called every frame
void AArenaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AArenaCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AArenaCharacter::LookRightRate);
}

bool AArenaCharacter::IsDead() const
{
	return Health <= 0;
}

float AArenaCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
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

	Weapon = GetWorld()->SpawnActor<AArenaWeapon>(weaponClass);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Weapon->SetOwner(this);
}

AArenaWeapon* AArenaCharacter::GetWeapon()
{
	return Weapon;
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
			Audio->SetSound(GruntingGotHitSound);
			Audio->Play();
		}
	}
	else // Character is dead
	{
		if (DeathScreamSound != nullptr)
		{
			Audio->SetSound(DeathScreamSound);
			Audio->Play();
		}

		//UCapsuleComponent* capsule = GetCapsuleComponent();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (OnDeath.IsBound()) OnDeath.Broadcast();
	}
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
}

void AArenaCharacter::IsBlockingStop()
{
	IsBlocking = false;
}

void AArenaCharacter::LeftFootstep()
{
	// add VFX coding for left foot

	if (FootstepSound != nullptr)
	{
		Audio->SetSound(FootstepSound);
		Audio->Play();
	}
}

void AArenaCharacter::RightFootstep()
{
	// add VFX coding for right foot

	if (FootstepSound != nullptr)
	{
		Audio->SetSound(FootstepSound);
		Audio->Play();
	}
}

bool AArenaCharacter::GetRunning()
{
	return Running;
}

void AArenaCharacter::Attack()
{
	if (Attacking || Blocking) return;
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
		Audio->SetSound(GruntingAttackSound);
		Audio->Play();
	}
	
	UE_LOG(LogTemp, Log, TEXT("%s attacked"), *GetName());
}

void AArenaCharacter::BlockStart()
{
	//UE_LOG(LogTemp, Warning, TEXT("Block started"));
	Blocking = true;
}

void AArenaCharacter::BlockStop()
{
	//if (Attacking || Blocking) return;
	//UE_LOG(LogTemp, Warning, TEXT("Block ended"));
	Blocking = false;
}

void AArenaCharacter::RunningStart()
{
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

void AArenaCharacter::MoveForward(float AxisValue)
{
	if (Attacking || Blocking) return;
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AArenaCharacter::MoveRight(float AxisValue)
{
	if (Attacking || Blocking) return;
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AArenaCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AArenaCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

