// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ArenaWeapon.generated.h"

UCLASS()
class GAD174_PROJECT3_API AArenaWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArenaWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void HitDetect();

	UFUNCTION(BlueprintPure)
	FVector GetTopPosition();

	UFUNCTION(BlueprintPure)
	FVector GetBottomPosition();

	UFUNCTION(BlueprintPure)
	bool IsOneHanded();

	UFUNCTION(BlueprintCallable)
	void AttackReset();

	UFUNCTION(BlueprintCallable)
	void PlayWeaponSwingSound();

	UFUNCTION(BlueprintPure)
	TSubclassOf<AActor> GetShieldClass();

	UFUNCTION(BlueprintPure)
	bool HasShield();

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* WeaponTop;

	UPROPERTY(EditAnywhere)
	USceneComponent* WeaponBottom;

	UPROPERTY(EditDefaultsOnly)
	bool ShowAttackDebug;

	UPROPERTY(EditDefaultsOnly)
	bool OneHanded;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> HitActors;

	UPROPERTY(EditAnywhere)
	float DamageAmount;

	/*UPROPERTY(EditDefaultsOnly)
	TArray< TEnumAsByte< EObjectTypeQuery >> ObjectTypesArray;*/

	UPROPERTY(EditDefaultsOnly)
	UAudioComponent* Audio;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* HitFleshSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* HitMetalSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* WeaponSwingSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* HitWallSound;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* VFX_HitMetal;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* VFX_HitFlesh;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ShieldClass;
};
