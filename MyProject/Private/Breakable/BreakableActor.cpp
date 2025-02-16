// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Items/Potion.h"
#include "Components/CapsuleComponent.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (bBroken) return;
	bBroken = true;
	if (HasTreasure() && HasPotions())
	{
		const int32 Selection = FMath::RandRange(0, 1);
		if (Selection == 0)
		{
			SetTreasureSpawnTimer();
		}
		else if (Selection == 1)
		{
			SetPotionSpawnTimer();
		}
	}
	else if (HasTreasure())
	{
		SetTreasureSpawnTimer();
	}
	else if (HasPotions())
	{
		SetPotionSpawnTimer();
	}
}

void ABreakableActor::SpawnRandomTreasure()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;

		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);

		World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}
}

void ABreakableActor::SpawnRandomPotion()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;

		const int32 Selection = FMath::RandRange(0, PotionClasses.Num() - 1);

		World->SpawnActor<APotion>(PotionClasses[Selection], Location, GetActorRotation());
	}
}

void ABreakableActor::SetTreasureSpawnTimer()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABreakableActor::SpawnRandomTreasure, 0.1f);
}

void ABreakableActor::SetPotionSpawnTimer()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABreakableActor::SpawnRandomPotion, 0.1f);
}

bool ABreakableActor::HasPotions()
{
	return PotionClasses.Num() > 0;
}

bool ABreakableActor::HasTreasure()
{
	return TreasureClasses.Num() > 0;
}

