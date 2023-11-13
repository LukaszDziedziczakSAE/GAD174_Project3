// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaWeapon.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AArenaWeapon::AArenaWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	WeaponTop = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Top"));
	WeaponTop->SetupAttachment(Root);

	WeaponBottom = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Bottom"));
	WeaponBottom->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AArenaWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArenaWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArenaWeapon::HitDetect()
{
	TArray< TEnumAsByte< EObjectTypeQuery >> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray< AActor* > ActorsToIgnore;
	EDrawDebugTrace::Type DrawDebugType;
	if (ShowAttackDebug)
	{
		DrawDebugType = EDrawDebugTrace::ForDuration;
	}
	else
	{
		DrawDebugType = EDrawDebugTrace::None;
	}
	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingleForObjects(this, GetTopPosition(), GetBottomPosition(), 10, ObjectTypesArray, false, ActorsToIgnore, DrawDebugType, HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.0);
}

FVector AArenaWeapon::GetTopPosition()
{
	return WeaponTop->GetComponentTransform().GetLocation();
}

FVector AArenaWeapon::GetBottomPosition()
{
	return WeaponBottom->GetComponentTransform().GetLocation();
}

