// Fill out your copyright notice in the Description page of Project Settings.


#include "menus/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"
#include "SlashSaveGame.h"

bool UMainMenuWidget::SaveExist()
{
	if (UGameplayStatics::DoesSaveGameExist("SaveSlot0", 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

TObjectPtr<USlashSaveGame> UMainMenuWidget::LoadSave()
{
	return Cast<USlashSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveSlot0", 0));
}

void UMainMenuWidget::DeleteSave()
{
	if (SaveExist())
	{
		UGameplayStatics::DeleteGameInSlot("SaveSlot0", 0);
	}
}

void UMainMenuWidget::OpenLevel(FName LevelName)
{
	if (LevelName.IsValid())
	{
		UGameplayStatics::OpenLevel(this, LevelName);
		SetGameMode();
	}
}

void UMainMenuWidget::ConstructMainMenu()
{
	SetUIMode();
	if (!SaveExist() && ContinueButton)
	{
		ContinueButton->SetIsEnabled(false);
	}

}

void UMainMenuWidget::SetUIMode()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetInputMode(FInputModeUIOnly());
}

void UMainMenuWidget::OnClickNewGame()
{
	DeleteSave();
	//OpenLevel(FName("NewOpenWorld"));
}

void UMainMenuWidget::SetGameMode()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
}

void UMainMenuWidget::OnClickContinue()
{
	if (SaveExist())
	{
		USlashSaveGame* LoadedGame = Cast<USlashSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveSlot0", 0));
		if (!LoadedGame->LatestLevelName.IsEmpty())
		{
			OpenLevel(FName(*LoadedGame->LatestLevelName));
		}
		else {
			UE_LOG(LogTemp, Display, TEXT("Latest level name is empty"));
		}
	}
}

void UMainMenuWidget::OnClickQuit()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}
