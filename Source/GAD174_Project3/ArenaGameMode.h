// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaSpawner.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNewRound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMatchEnd);

/**
 * 
 */
UCLASS()
class GAD174_PROJECT3_API AArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StartNextRound();

	UFUNCTION(BlueprintPure)
	int GetRound();

	UFUNCTION()
	void OnCharacterDeath();

	UPROPERTY(EditAnywhere)
	float RoundCooldown;

	UPROPERTY(BlueprintAssignable)
	FNewRound OnNewRound;

	UPROPERTY(BlueprintAssignable)
	FMatchEnd OnMatchEnd;

	UFUNCTION(BlueprintCallable)
	void NewMatch();

	UFUNCTION(BlueprintCallable)
	bool MatchStarted();
	
private:
	UPROPERTY(VisibleAnywhere)
	int round;

	UPROPERTY(VisibleAnywhere)
	int HighestRound;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> Spawners;

	UPROPERTY(VisibleAnywhere)
	AArenaCharacter* Player;

	UPROPERTY(VisibleAnywhere)
	TArray<AArenaCharacter*> Enemies;

	UFUNCTION()
	bool AllEnemiesDead();

	UPROPERTY()
	FTimerHandle RoundCooldownTimer;

	UPROPERTY(VisibleAnywhere)
	FVector PlayerStartLocation;

	UPROPERTY(VisibleAnywhere)
	FRotator PlayerStartRotation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
