// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaSpawner.h"


// Sets default values
AArenaSpawner::AArenaSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArenaSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArenaSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AArenaCharacter* AArenaSpawner::Spawn(int RoundNumber)
{
	switch (RoundNumber)
	{
	case 1:
		if (Round1Enemy != nullptr) return SpawnEnemy(Round1Enemy);
		else return nullptr;

	case 2:
		if (Round2Enemy != nullptr) return SpawnEnemy(Round2Enemy);
		else return nullptr;

	case 3:
		if (Round3Enemy != nullptr) return SpawnEnemy(Round3Enemy);
		else return nullptr;

	case 4:
		if (Round4Enemy != nullptr) return SpawnEnemy(Round4Enemy);
		else return nullptr;

	case 5:
		if (Round5Enemy != nullptr) return SpawnEnemy(Round5Enemy);
		else return nullptr;

	case 6:
		if (Round6Enemy != nullptr) return SpawnEnemy(Round6Enemy);
		else return nullptr;

	case 7:
		if (Round7Enemy != nullptr) return SpawnEnemy(Round7Enemy);
		else return nullptr;

	case 8:
		if (Round8Enemy != nullptr) return SpawnEnemy(Round8Enemy);
		else return nullptr;

	case 9:
		if (Round9Enemy != nullptr) return SpawnEnemy(Round9Enemy);
		else return nullptr;

	case 10:
		if (Round10Enemy != nullptr) return SpawnEnemy(Round10Enemy);
		else return nullptr;
	default:
		return nullptr;
	}
}

int AArenaSpawner::GetHighestRoundNumber()
{
	int count = 0;

	if (Round1Enemy != nullptr) count = 1;
	if (Round2Enemy != nullptr) count = 2;
	if (Round3Enemy != nullptr) count = 3;
	if (Round4Enemy != nullptr) count = 4;
	if (Round5Enemy != nullptr) count = 5;
	if (Round6Enemy != nullptr) count = 6;
	if (Round7Enemy != nullptr) count = 7;
	if (Round8Enemy != nullptr) count = 8;
	if (Round9Enemy != nullptr) count = 9;
	if (Round10Enemy != nullptr) count = 10;

	return count;
}

AArenaCharacter* AArenaSpawner::SpawnEnemy(TSubclassOf<AArenaCharacter> Enemy)
{
	AArenaCharacter* enemy = GetWorld()->SpawnActor<AArenaCharacter>(Enemy);
	FTransform transform = GetTransform();
	FVector location = transform.GetLocation();
	location.Z += 90;
	transform.SetLocation(location);
	enemy->SetActorTransform(transform);
	return enemy;
}





