// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Potion.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API APotion : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Potion Properties")
	int32 HealthIncrease = 0;

	UPROPERTY(EditAnywhere, Category = "Potion Properties")
	int32 StaminaIncrease = 0;

public:
	FORCEINLINE int32 GetHealthIncrease() const { return HealthIncrease; }
	FORCEINLINE int32 GetStaminaIncrease() const { return StaminaIncrease; }
	
};
