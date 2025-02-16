// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObjects/InteractableObject.h"
#include "SavePoint.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class MYPROJECT_API ASavePoint : public AInteractableObject
{
	GENERATED_BODY()
	
public:	
	ASavePoint();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact() override;
	void ShowSaveWidget();
	void HideSaveWidget();


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> FlameParticles;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> SaveWidgetClass;

	UPROPERTY()
	UUserWidget* SaveWidget;

private:
	FTimerHandle SaveWidgetTimer;

};
