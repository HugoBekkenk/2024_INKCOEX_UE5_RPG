// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class UButton;

UCLASS()
class MYPROJECT_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

	
protected:
	UFUNCTION(BlueprintCallable)
	void ConstructPauseWidget();

	UFUNCTION(BlueprintCallable)
	void OnClickContinue();

	UFUNCTION(BlueprintCallable)
	void OnClickMainMenu();

	UFUNCTION(BlueprintCallable)
	void OnClickQuit();

	void SetUIMode();

	void SetGameMode();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ContinueButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
};
