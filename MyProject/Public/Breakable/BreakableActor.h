// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"


class UGeometryCollectionComponent;
class UCapsuleComponent;
class ATreasure;
class APotion;

UCLASS()
class MYPROJECT_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> Capsule;

private:
	void SpawnRandomTreasure();
	void SpawnRandomPotion();
	void SetTreasureSpawnTimer();
	void SetPotionSpawnTimer();
	bool HasPotions();
	bool HasTreasure();

	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<ATreasure>> TreasureClasses;

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<APotion>> PotionClasses;

	bool bBroken = false;
};
