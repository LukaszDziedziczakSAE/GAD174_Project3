// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaWeapon.h"
#include "ArenaCharacter.h"
#include "Components/AudioComponent.h"
#include "NiagaraFunctionLibrary.h"
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

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(Root);
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
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray< AActor* > ActorsToIgnore;
	ActorsToIgnore.Add(Owner);
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
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, GetTopPosition(), GetBottomPosition(), 10, ObjectTypesArray, true, ActorsToIgnore, DrawDebugType, HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.0))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit detected"));

		if (!HitActors.Contains(HitResult.GetActor()))
		{
			HitActors.Add(HitResult.GetActor());

			AArenaCharacter* HitCharacter = Cast<AArenaCharacter>(HitResult.GetActor());
			if (HitCharacter != nullptr && !HitCharacter->IsDead())
			{
				UE_LOG(LogTemp, Log, TEXT("%s Hit %s"), *GetName(), *HitCharacter->GetName());
				

				if (HitCharacter->Blocking)
				{
					if (HitMetalSound != nullptr)
					{
						Audio->SetSound(HitMetalSound);
						Audio->Play();
					}
					

					if (VFX_HitMetal != nullptr)
					{
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFX_HitMetal, HitResult.Location);
					}
					
					HitCharacter->PlayImpact();
				}
				else 
				{
					if (HitFleshSound != nullptr)
					{
						Audio->SetSound(HitFleshSound);
						Audio->Play();
					}

					if (VFX_HitFlesh != nullptr)
					{
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFX_HitFlesh, HitResult.Location);
					}
					
					HitCharacter->ApplyDamage(DamageAmount);
				}
			}
		}
		/*else
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor already hit"));
		}*/
	}
}

FVector AArenaWeapon::GetTopPosition()
{
	return WeaponTop->GetComponentTransform().GetLocation();
}

FVector AArenaWeapon::GetBottomPosition()
{
	return WeaponBottom->GetComponentTransform().GetLocation();
}

bool AArenaWeapon::IsOneHanded()
{
	return OneHanded;
}

void AArenaWeapon::AttackReset()
{
	HitActors.Empty();
}

void AArenaWeapon::PlayWeaponSwingSound()
{
	if (WeaponSwingSound != nullptr)
	{
		Audio->SetSound(WeaponSwingSound);
		Audio->Play();
	}
}

TSubclassOf<AArenaShield> AArenaWeapon::GetShieldClass()
{
	return ShieldClass;
}

bool AArenaWeapon::HasShield()
{
	if (!OneHanded) return false;
	return ShieldClass != nullptr;
}

float AArenaWeapon::GetStaminaAttackCost()
{
	return StaminaAttackCost;
}

float AArenaWeapon::GetStaminaBlockingStartCost()
{
	return StaminaBlockingStartCost;
}

float AArenaWeapon::GetStaminaBlockingCost()
{
	return StaminaBlockingCost;
}

