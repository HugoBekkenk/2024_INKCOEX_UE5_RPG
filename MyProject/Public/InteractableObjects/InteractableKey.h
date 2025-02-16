// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObjects/InteractableObject.h"
#include "InteractableKey.generated.h"

class UKeyWidget;
class USoundBase;

UCLASS()
class MYPROJECT_API AInteractableKey : public AInteractableObject
{
	GENERATED_BODY()

public:
	virtual void Interact() override;


	UPROPERTY(EditAnywhere, Category = "Key Properties")
	FString UnlockedDoor;

protected:

	UPROPERTY(EditAnywhere)
	FString LevelName;

	UPROPERTY(EditAnywhere)
	FTransform LevelStartTransform;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UKeyWidget> KeyWidgetClass;

	UPROPERTY()
	UKeyWidget* KeyWidget;
	
public:
	FORCEINLINE FTransform GetLevelStartTransform() const { return LevelStartTransform; }
	FORCEINLINE FString GetLevelName() const { return LevelName; }
};
