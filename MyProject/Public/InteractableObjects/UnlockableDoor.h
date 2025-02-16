// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "UnlockableDoor.generated.h"

class USoundBase;

UCLASS()
class MYPROJECT_API AUnlockableDoor : public ADoor
{
	GENERATED_BODY()

public:
	AUnlockableDoor();

	UFUNCTION(BlueprintCallable)
	void DoorOpened();

	void DoorClosed();

	void ShowLockedWidget();
	void HideLockedWidget();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door Properties")
	FString DoorId;

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> LockedWidgetClass;

	UPROPERTY()
	UUserWidget* LockedWidget;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LockedSound;

private:
	FTimerHandle LockedWidgetTimer;
};
