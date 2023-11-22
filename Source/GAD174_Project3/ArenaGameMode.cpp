// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void AArenaGameMode::StartNextRound()
{

	// Remove dead enemies
	if (Enemies.Num() > 0)
	{
		for (int index = 0; index < Enemies.Num(); index++)
		{
			Enemies[index]->GetWeapon()->Destroy();
			Enemies[index]->Destroy();
		}
		Enemies.Empty();
	}

	round++;

	if (round > HighestRound)
	{
		if (OnMatchEnd.IsBound()) OnMatchEnd.Broadcast();
		return;
	}


	/*TArray<AActor*> weapons;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaWeapon::StaticClass(), weapons);
	if (weapons.Num() > 0)
	{
		for (int index = 0; index < weapons.Num(); index++)
		{
			if (weapons[index]->Owner != nullptr)
			{
				weapons[index]->Destroy();
			}
			
		}
	}*/

	// Spawn new enemies
	for (int index = 0; index < Spawners.Num(); index++)
	{
		AArenaSpawner* spawner = Cast<AArenaSpawner>(Spawners[index]);
		 AArenaCharacter* Enemy = spawner->Spawn(round);
		 if (Enemy != nullptr)
		 {
			 Enemies.Add(Enemy);
			 Enemy->OnDeath.AddDynamic(this, &AArenaGameMode::OnCharacterDeath);
		 }
		 
	}

	if (OnNewRound.IsBound()) OnNewRound.Broadcast();
}

int AArenaGameMode::GetRound()
{
	return round;
}

void AArenaGameMode::OnCharacterDeath()
{
	//UE_LOG(LogTemp, Warning, TEXT("Character Died"));

	if (AllEnemiesDead())
	{
		GetWorld()->GetTimerManager().SetTimer(RoundCooldownTimer, this, &AArenaGameMode::StartNextRound, RoundCooldown, false);
	}
}

bool AArenaGameMode::AllEnemiesDead()
{
	if (Enemies.Num() > 0)
	{
		for (int index = 0; index < Enemies.Num(); index++)
		{
			if (!Enemies[index]->IsDead()) return false;
		}
	}

	return true;
}

void AArenaGameMode::BeginPlay()
{
	Super::BeginPlay();
	round = 0;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaSpawner::StaticClass(), Spawners);

	// Determine highest round number
	HighestRound = 0;
	for (int index = 0; index < Spawners.Num(); index++)
	{
		AArenaSpawner* spawner = Cast<AArenaSpawner>(Spawners[index]);
		int spawnerHighest = spawner->GetHighestRoundNumber();
		if (spawnerHighest > HighestRound) HighestRound = spawnerHighest;
	}
}
