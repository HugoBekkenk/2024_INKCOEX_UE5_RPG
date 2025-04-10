// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

class MYPROJECT_API IHitInterface
{
	GENERATED_BODY()

public:
	// Make pure function with the = 0 if virtual
	//virtual void GetHit(const FVector& ImpactPoint) = 0;
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint, AActor* Hitter);
};
