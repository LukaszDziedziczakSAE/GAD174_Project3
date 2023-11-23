// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShield.h"

// Sets default values
AArenaShield::AArenaShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AArenaShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArenaShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArenaShield::CollisionEnabled(bool Enabled)
{
	if (Enabled)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

