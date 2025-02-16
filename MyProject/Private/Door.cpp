// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	/*DoorMeshBackground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMeshBackgroundComponent"));
	DoorMeshBackground->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	DoorMeshBackground->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = DoorMeshBackground;*/

	DoorMeshRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMeshRightComponent"));
	DoorMeshRight->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	DoorMeshRight->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DoorMeshRight->SetupAttachment(GetRootComponent());
	DoorRightOriginalRotation = DoorMeshRight->GetComponentRotation();

	DoorMeshLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMeshLeftComponent"));
	DoorMeshLeft->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	DoorMeshLeft->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DoorMeshLeft->SetupAttachment(GetRootComponent());
	DoorLeftOriginalRotation = DoorMeshLeft->GetComponentRotation();

	/*Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());*/
	//Sphere->SetGenerateOverlapEvents(true);

}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Opening && DoorMeshRight->GetComponentRotation().Yaw > (DoorRightOriginalRotation.Yaw - 110.f))
	{
		float RotationRateLeft = 55.f;
		float RotationRateRight = -55.f;

		DoorMeshLeft->AddRelativeRotation(FRotator(0.f, RotationRateLeft * DeltaTime, 0.f));
		DoorMeshRight->AddRelativeRotation(FRotator(0.f, RotationRateRight * DeltaTime, 0.f));

	}
}

void ADoor::Interact()
{
	Super::Interact();
	Opening = true;

	DoorMeshLeft->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	DoorMeshRight->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	if (!LevelName.IsEmpty() && LevelStartTransform.IsValid())
	{
		GetWorldTimerManager().SetTimer(LevelTimer, this, &ADoor::ChangeLevel, 1.9f);
	}
}

void ADoor::ChangeLevel()
{
	if (!LevelName.IsEmpty())
	{
		UGameplayStatics::OpenLevel(this, FName(LevelName));
	}
}


