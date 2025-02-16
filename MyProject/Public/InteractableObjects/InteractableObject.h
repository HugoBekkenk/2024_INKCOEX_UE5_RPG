// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

class USphereComponent;
class USoundBase;

UCLASS()
class MYPROJECT_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableObject();

	virtual void Tick(float DeltaTime) override;

	virtual void Interact();

	virtual void SpawnInteractionSound();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> ObjectMesh;

	UPROPERTY(VisibleAnywhere, Category = Components)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> InteractWidgetClass;

	UPROPERTY()
	UUserWidget* InteractWidget;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> InteractionSound;

};
