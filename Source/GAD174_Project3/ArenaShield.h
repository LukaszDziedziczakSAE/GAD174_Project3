// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaShield.generated.h"

UCLASS()
class GAD174_PROJECT3_API AArenaShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArenaShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UFUNCTION(BlueprintCallable)
	void CollisionEnabled(bool Enabled);

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

};
