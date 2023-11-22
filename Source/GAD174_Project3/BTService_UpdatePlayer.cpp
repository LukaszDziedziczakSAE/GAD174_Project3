// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdatePlayer.h"
#include <Kismet/GameplayStatics.h>
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdatePlayer::UBTService_UpdatePlayer()
{
	NodeName = TEXT("Update Player");
}

void UBTService_UpdatePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn == nullptr) return;

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);

}
