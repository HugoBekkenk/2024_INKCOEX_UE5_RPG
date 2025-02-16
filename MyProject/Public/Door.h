// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObjects/InteractableObject.h"
#include "Door.generated.h"

class USphereComponent;

UCLASS()
class MYPROJECT_API ADoor : public AInteractableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> DoorMeshLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> DoorMeshRight;

	UPROPERTY(EditAnywhere)
	FString LevelName;

	UPROPERTY(EditAnywhere)
	FTransform LevelStartTransform;

private:
	void ChangeLevel();
	FTimerHandle LevelTimer;
	bool Opening = false;

	FRotator DoorLeftOriginalRotation;
	FRotator DoorRightOriginalRotation;

public:
	FORCEINLINE FTransform GetLevelStartTransform() const { return LevelStartTransform; }
	FORCEINLINE FString GetLevelName() const { return LevelName; }
};