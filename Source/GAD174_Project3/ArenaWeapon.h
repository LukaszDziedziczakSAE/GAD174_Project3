// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
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
};
