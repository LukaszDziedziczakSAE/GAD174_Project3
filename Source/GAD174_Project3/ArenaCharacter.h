// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArenaWeapon.h"
#include "ArenaCharacter.generated.h"

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

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

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

private:
	void Attack();
	void BlockStart();
	void BlockStop();
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

	// Is the player pressing the blocking button
	UPROPERTY(VisibleAnywhere)
	bool Blocking;

	// Is the player pressing the blocking button
	UPROPERTY(VisibleAnywhere)
	bool Attacking;

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
};
