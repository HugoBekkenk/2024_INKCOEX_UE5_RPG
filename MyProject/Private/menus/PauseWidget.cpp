// Fill out your copyright notice in the Description page of Project Settings.


#include "menus/PauseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/SlashCharacter.h"

void UPauseWidget::ConstructPauseWidget()
{
	SetUIMode();
}

void UPauseWidget::OnClickContinue()
{
	SetGameMode();
	RemoveFromParent();
}

void UPauseWidget::OnClickMainMenu()
{
	UGameplayStatics::OpenLevel(this, FName("MainMenuMap"));
}

void UPauseWidget::OnClickQuit()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}

void UPauseWidget::SetUIMode()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 0.f);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetInputMode(FInputModeUIOnly());
}

void UPauseWidget::SetGameMode()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
	ASlashCharacter* Character = Cast<ASlashCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		Character->PauseWidget = nullptr;
	}
}
