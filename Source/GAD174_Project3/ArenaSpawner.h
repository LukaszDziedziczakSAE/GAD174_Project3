// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaCharacter.h"
#include "GameFramework/Actor.h"
#include "ArenaSpawner.generated.h"

UCLASS()
class GAD174_PROJECT3_API AArenaSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArenaSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	AArenaCharacter* Spawn(int RoundNumber);

	UFUNCTION(BlueprintCallable)
	int GetHighestRoundNumber();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AArenaCharacter> Round1Enemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AArenaCharacter> Round2Enemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AArenaCharacter> Round3Enemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AArenaCharacter> Round4Enemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AArenaCharacter> Round5Enemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AArenaCharacter> Round6Enemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AArenaCharacter> Round7Enemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AArenaCharacter> Round8Enemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AArenaCharacter> Round9Enemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AArenaCharacter> Round10Enemy;

	UFUNCTION()
	AArenaCharacter* SpawnEnemy(TSubclassOf<AArenaCharacter> Enemy);

};
