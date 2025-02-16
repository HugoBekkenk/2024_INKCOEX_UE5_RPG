// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class USlashSaveGame;

UCLASS()
class MYPROJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool SaveExist();
	TObjectPtr<USlashSaveGame> LoadSave();
	void DeleteSave();
	void OpenLevel(FName LevelName);
	void SetUIMode();
	void SetGameMode();

protected:

	UFUNCTION(BlueprintCallable)
	void ConstructMainMenu();

	UFUNCTION(BlueprintCallable)
	void OnClickNewGame();

	UFUNCTION(BlueprintCallable)
	void OnClickContinue();

	UFUNCTION(BlueprintCallable)
	void OnClickQuit();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NewGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ContinueButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
};
