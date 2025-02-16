// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObjects/SavePoint.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Blueprint/UserWidget.h"

ASavePoint::ASavePoint()
{
	PrimaryActorTick.bCanEverTick = true;

	FlameParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Flames"));
	FlameParticles->SetupAttachment(GetRootComponent());

}

void ASavePoint::BeginPlay()
{
	Super::BeginPlay();
	if (FlameParticles)
	{
		FlameParticles->Deactivate();
	}
	
}

void ASavePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASavePoint::Interact()
{
	Super::Interact();
	if (FlameParticles)
	{
		FlameParticles->Activate();
	}
}

void ASavePoint::ShowSaveWidget()
{
	if (SaveWidgetClass && !SaveWidget)
	{
		SaveWidget = CreateWidget<UUserWidget>(GetWorld(), SaveWidgetClass);
		if (SaveWidget)
		{
			SaveWidget->AddToViewport();
			GetWorldTimerManager().SetTimer(SaveWidgetTimer, this, &ASavePoint::HideSaveWidget, 2.f);

		}
	}
}

void ASavePoint::HideSaveWidget()
{
	if (SaveWidget)
	{
		SaveWidget->RemoveFromParent();
		SaveWidget = nullptr;
	}
}
