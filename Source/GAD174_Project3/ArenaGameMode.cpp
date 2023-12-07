// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "ArenaCharacter.h"

void AArenaGameMode::StartNextRound()
{

	// Remove dead enemies
	if (Enemies.Num() > 0)
	{
		for (int index = 0; index < Enemies.Num(); index++)
		{
			Enemies[index]->GetWeapon()->Destroy();
			if (Enemies[index]->GetShield() != nullptr) Enemies[index]->GetShield()->Destroy();
			Enemies[index]->Destroy();
		}
		Enemies.Empty();
	}

	round++;

	if (round > HighestRound || Player->IsDead())
	{
		round = 0;
		if (OnMatchEnd.IsBound()) OnMatchEnd.Broadcast();
		return;
	}

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

	if (AllEnemiesDead() || Player->IsDead())
	{
		GetWorld()->GetTimerManager().SetTimer(RoundCooldownTimer, this, &AArenaGameMode::StartNextRound, RoundCooldown, false);
	}
}

void AArenaGameMode::NewMatch()
{
	if (Enemies.Num() > 0)
	{
		for (int index = 0; index < Enemies.Num(); index++)
		{
			Enemies[index]->GetWeapon()->Destroy();
			Enemies[index]->Destroy();
		}
		Enemies.Empty();
	}

	if (Player == nullptr) return;

	Player->SetActorLocationAndRotation(PlayerStartLocation, PlayerStartRotation);
	Player->ResetCharacter();
}

bool AArenaGameMode::MatchStarted()
{
	return round > 0 && round <= HighestRound;
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

	Player = Cast<AArenaCharacter>(GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator());
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode could not get Player Character"));
	}
	else
	{
		Player->OnDeath.AddDynamic(this, &AArenaGameMode::OnCharacterDeath);
		PlayerStartLocation = Player->GetActorLocation();
		PlayerStartRotation = Player->GetActorRotation();
	}
	
}
