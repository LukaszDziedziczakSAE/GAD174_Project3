// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateRunning.generated.h"

/**
 * 
 */
UCLASS()
class GAD174_PROJECT3_API UBTService_UpdateRunning : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdateRunning();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
