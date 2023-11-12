// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaCharacter.h"

// Sets default values
AArenaCharacter::AArenaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArenaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
}

void AArenaCharacter::Attack()
{
	if (Blocking) return;
	//UE_LOG(LogTemp, Warning, TEXT("Attack pressed"));
	Attacking = true;
}

void AArenaCharacter::BlockStart()
{
	//UE_LOG(LogTemp, Warning, TEXT("Block started"));
	Blocking = true;
}

void AArenaCharacter::BlockStop()
{
	//UE_LOG(LogTemp, Warning, TEXT("Block ended"));
	Blocking = false;
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

