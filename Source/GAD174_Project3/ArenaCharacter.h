// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArenaWeapon.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ArenaCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeath);

UCLASS()
class GAD174_PROJECT3_API AArenaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AArenaCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	// Is the character blocking attacks
	UPROPERTY(VisibleAnywhere)
	bool IsBlocking;

	UFUNCTION(BlueprintPure)
	bool GetBlockingPressed();

	UFUNCTION(BlueprintPure)
	bool GetAttacking();

	UFUNCTION(BlueprintCallable)
	void AttackComplete();

	UFUNCTION(BlueprintCallable)
	void SpawnWeapon(TSubclassOf<AArenaWeapon> weaponClass);

	UFUNCTION(BlueprintCallable)
	AArenaWeapon* GetWeapon();

	UFUNCTION(BlueprintPure)
	bool IsWeaponOneHanded();

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float Amount);

	UFUNCTION(BlueprintCallable)
	void PlayImpact();

	UFUNCTION(BlueprintCallable)
	void IsBlockingStart();

	UFUNCTION(BlueprintCallable)
	void IsBlockingStop();

	UFUNCTION(BlueprintCallable)
	void LeftFootstep();

	UFUNCTION(BlueprintCallable)
	void RightFootstep();

	UFUNCTION(BlueprintPure)
	bool GetRunning();

	UPROPERTY(BlueprintAssignable)
	FDeath OnDeath;

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void BlockStart();

	UFUNCTION()
	void BlockStop();

	UFUNCTION()
	void RunningStart();

	UFUNCTION()
	void RunningStop();
	
	// Is the player pressing the blocking button
	UPROPERTY(VisibleAnywhere)
	bool Blocking;

	UFUNCTION(BlueprintPure)
	float GetHealthPercentage();

	UFUNCTION(BlueprintPure)
	float GetStaminaPercentage();

	UFUNCTION(BlueprintCallable)
	void ResetCharacter();

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);

	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	float MaxStamina = 100;

	UPROPERTY(VisibleAnywhere)
	float Stamina;

	UPROPERTY(EditDefaultsOnly)
	float StaminaRegen = 10;

	// Is the player pressing the blocking button
	UPROPERTY(VisibleAnywhere)
	bool Attacking;

	// Is the player pressing the run/sprint button
	UPROPERTY(VisibleAnywhere)
	bool Running;

	UPROPERTY(VisibleAnywhere)
	AArenaWeapon* Weapon;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AArenaWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TwoHandedAttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* OneHandedAttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TwoHandedImpactMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* OneHandedImpactMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TwoHandedBlockingImpactMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* OneHandedBlockingImpactMontage;

	UPROPERTY(EditDefaultsOnly)
	UAudioComponent* Audio;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* GruntingAttackSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* GruntingGotHitSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* DeathScreamSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* FootstepSound;

	UPROPERTY(EditDefaultsOnly)
	float RunningSpeed;

	UPROPERTY()
	float WalkingSpeed;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* VFX_Footstep;

	UPROPERTY()
	class AArenaGameMode* GameMode;
};
