// Fill out your copyright notice in the Description page of Project Settings.


#include "menus/KeyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"
#include "Characters/SlashCharacter.h"

void UKeyWidget::ConstructKeyWidget()
{
	SetUIMode();
}

void UKeyWidget::OnClickTeleport()
{
	OpenLevel();
	RemoveFromParent();
}

void UKeyWidget::OnClickDismiss()
{
	SetGameMode();
	RemoveFromParent();
}

void UKeyWidget::SetUIMode()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 0.f);
	//UGameplayStatics::SetGamePaused(this, true);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetInputMode(FInputModeUIOnly());
}

void UKeyWidget::SetGameMode()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
	//UGameplayStatics::SetGamePaused(this, false);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
}

void UKeyWidget::OpenLevel()
{
	SetGameMode();
	UGameplayStatics::OpenLevel(this, FName("NewOpenWorld"));
}
