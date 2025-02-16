// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObjects/InteractableKey.h"
#include "Kismet/GameplayStatics.h"
#include "menus/KeyWidget.h"

void AInteractableKey::Interact()
{
	if (!LevelName.IsEmpty() && LevelStartTransform.IsValid())
	{
		if (KeyWidgetClass && !KeyWidget)
		{
			KeyWidget = CreateWidget<UKeyWidget>(GetWorld(), KeyWidgetClass);
			if (KeyWidget)
			{
				KeyWidget->AddToViewport();
			}
		}
	}
	else {
		Super::Interact();
		this->Destroy();
	}
}
