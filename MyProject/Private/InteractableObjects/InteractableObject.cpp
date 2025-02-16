// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObjects/InteractableObject.h"
#include "Components/SphereComponent.h"
#include "Characters/SlashCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


AInteractableObject::AInteractableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMeshComponent"));
	ObjectMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ObjectMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = ObjectMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

}

void AInteractableObject::Tick(float DeltaTime)
{
	
}

void AInteractableObject::Interact()
{
	if (InteractWidget)
	{
		SpawnInteractionSound();
		InteractWidget->RemoveFromParent();
		InteractWidget = nullptr;
	}
}

void AInteractableObject::SpawnInteractionSound()
{
	if (InteractionSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			InteractionSound,
			GetActorLocation()
		);
	}
}

void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AInteractableObject::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddUniqueDynamic(this, &AInteractableObject::OnSphereEndOverlap);
	
}

void AInteractableObject::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* Character = Cast<ASlashCharacter>(OtherActor);
	if (Character && InteractWidgetClass)
	{
		if (!InteractWidget)
		{
			InteractWidget = CreateWidget<UUserWidget>(GetWorld(), InteractWidgetClass);
			if (InteractWidget)
			{
				InteractWidget->AddToViewport();
			}
		}
		Character->OverlappingObject = this;
	}
}

void AInteractableObject::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASlashCharacter* Character = Cast<ASlashCharacter>(OtherActor);
	if (Character)
	{
		Character->OverlappingObject = nullptr;
		if (InteractWidget)
		{
			InteractWidget->RemoveFromParent();
			InteractWidget = nullptr;
		}
	}
}


