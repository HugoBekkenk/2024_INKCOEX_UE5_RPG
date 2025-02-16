// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SlashSaveGame.generated.h"

class AWeapon;

UCLASS()
class MYPROJECT_API USlashSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	USlashSaveGame();

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Level;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	FString EquippedWeaponId;

	UPROPERTY(VisibleAnywhere, Category = "Actor Attributes")
	FString LatestLevelName;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	FTransform LatestTeleportTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Attributes")
	TArray<FString> UnlockedDoors;

	UPROPERTY(BlueprintReadOnly, Category = "Actor Attributes")
	TArray<FString> OpenedDoors;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	/*UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes;*/

//public:
	//FORCEINLINE UAttributeComponent* GetAttributes() const { return Attributes; }

	//FORCEINLINE void SetAttributes(UAttributeComponent* NewAttributes) { Attributes = NewAttributes; }

};
