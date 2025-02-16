// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyWidget.generated.h"

class UButton;

UCLASS()
class MYPROJECT_API UKeyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void ConstructKeyWidget();

	UFUNCTION(BlueprintCallable)
	void OnClickTeleport();

	UFUNCTION(BlueprintCallable)
	void OnClickDismiss();

	void SetUIMode();

	void SetGameMode();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> TeleportButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DismissButton;

	void OpenLevel();
	FTimerHandle LevelTimer;
	
};
