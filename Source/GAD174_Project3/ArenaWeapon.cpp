// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaWeapon.h"

// Sets default values
AArenaWeapon::AArenaWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AArenaWeapon::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AArenaWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArenaWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap"));
}

// Called every frame
void AArenaWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

