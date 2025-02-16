// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SlashGameInstance.generated.h"

class USlashOverlay;
class UAttributeComponent;

UCLASS()
class MYPROJECT_API USlashGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	//UPROPERTY()
	//TObjectPtr<USlashOverlay> SlashOverlay;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes;

public:
	FORCEINLINE UAttributeComponent* GetAttributes() const { return Attributes; }

	FORCEINLINE void SetAttributes(UAttributeComponent* NewAttributes) { Attributes = NewAttributes; }
	
};
