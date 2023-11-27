// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateRunning.h"
#include <Kismet/GameplayStatics.h>
#include "ArenaCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateRunning::UBTService_UpdateRunning()
{
	NodeName = TEXT("Update Running");
}

void UBTService_UpdateRunning::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn == nullptr) return;

	AArenaCharacter* Character = Cast<AArenaCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("Missing Character referance in UpdateRunning");
		return;
	}
	
	double distance = FVector::Dist(PlayerPawn->GetActorLocation(), Character->GetActorLocation());

	Character->SetRunning(distance > 500);
}
