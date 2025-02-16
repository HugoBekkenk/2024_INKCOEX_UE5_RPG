// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObjects/UnlockableDoor.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AUnlockableDoor::AUnlockableDoor()
{
	ObjectMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ObjectMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ObjectMesh->SetVisibility(false);
}

void AUnlockableDoor::DoorOpened()
{
	DoorMeshLeft->AddLocalRotation(FRotator(0.f, 110, 0.f));
	DoorMeshRight->AddLocalRotation(FRotator(0.f, -110.f, 0.f));
}

void AUnlockableDoor::DoorClosed()
{
	DoorMeshLeft->AddLocalRotation(FRotator(0.f, 0.f, 0.f));
	DoorMeshRight->AddLocalRotation(FRotator(0.f, 0.f, 0.f));
}

void AUnlockableDoor::ShowLockedWidget()
{
	if (LockedWidgetClass && !LockedWidget)
	{
		LockedWidget = CreateWidget<UUserWidget>(GetWorld(), LockedWidgetClass);
		if (LockedWidget)
		{
			LockedWidget->AddToViewport();
			if (LockedSound)
			{
				UGameplayStatics::SpawnSoundAtLocation(
					this,
					LockedSound,
					GetActorLocation()
				);
			}
			GetWorldTimerManager().SetTimer(LockedWidgetTimer, this, &AUnlockableDoor::HideLockedWidget, 2.f);

		}
	}
}

void AUnlockableDoor::HideLockedWidget()
{
	if (LockedWidget)
	{
		LockedWidget->RemoveFromParent();
		LockedWidget = nullptr;
	}
}

