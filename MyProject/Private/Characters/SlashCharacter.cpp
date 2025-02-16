// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GroomComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Items/Potion.h"
#include "Kismet/GameplayStatics.h"
#include "Door.h"
#include "SlashSaveGame.h"
#include "InteractableObjects/SavePoint.h"
#include "InteractableObjects/InteractableKey.h"
#include "InteractableObjects/UnlockableDoor.h"
#include "menus/PauseWidget.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	/*CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);*/

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Attributes && SlashOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Pause);
		
	}

}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDodging()) return 0.f;
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsDodging()) return;
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	if (Attributes && Attributes->IsAlive())
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void ASlashCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ASlashCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && SlashOverlay)
	{
		if (Attributes->GetLevel() <= 1 && Attributes->GetSouls() <= 0)
		{
			Soul->ShowTutorialWidget();
		}
		Attributes->AddSouls(Soul->GetSouls());
		while (Attributes->GetSouls() >= 5 && Attributes->GetLevel() < 99)
		{
			if (Attributes->LevelUp())
			{
				SlashOverlay->SetLevel(Attributes->GetLevel());
				SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				Attributes->SetSouls(Attributes->GetSouls() - 5);
				if (LevelUpSound)
				{
					UGameplayStatics::PlaySound2D(this, LevelUpSound);
				}
			}
		}
		SlashOverlay->SetSouls(Attributes->GetSouls());
	}
}

void ASlashCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		SlashOverlay->SetGold(Attributes->GetGold());
	}
}

void ASlashCharacter::AddHealth(APotion* Potion)
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddHealth(Potion->GetHealthIncrease());
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void ASlashCharacter::AddStamina(APotion* Potion)
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddStamina(Potion->GetStaminaIncrease());
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeSave();
	InitializeCharacter();
	
}

void ASlashCharacter::InitializeSave()
{
	if (UGameplayStatics::DoesSaveGameExist("SaveSlot0", 0))
	{
		LoadSave();
	}
	else
	{
		LatestLevelName = UGameplayStatics::GetCurrentLevelName(this);
		LatestTeleportTransform = GetActorTransform();
		SaveGame();
	}
}

void ASlashCharacter::InitializeCharacter()
{
	Tags.Add(ActorTag);
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		InitializeSlashOverlay(PlayerController);
		InitializeInputMapping(PlayerController);
	}
	InitializeCameraManager();
	CameraFadeIn(3.f);
}

void ASlashCharacter::SaveGame()
{
	if (USlashSaveGame* SaveGameInstance = Cast<USlashSaveGame>(UGameplayStatics::CreateSaveGameObject(USlashSaveGame::StaticClass())))
	{
		SaveGameInstance->Health = Attributes->GetHealth();
		SaveGameInstance->MaxHealth = Attributes->GetMaxHealth();
		SaveGameInstance->MaxStamina = Attributes->GetMaxStamina();
		SaveGameInstance->Gold = Attributes->GetGold();
		SaveGameInstance->Souls = Attributes->GetSouls();
		SaveGameInstance->Level = Attributes->GetLevel();

		if (EquippedWeapon && EquippedWeapon->GetWeaponId() != "")
		{
			SaveGameInstance->EquippedWeaponId = EquippedWeapon->GetWeaponId();
		}

		SaveGameInstance->LatestTeleportTransform = LatestTeleportTransform;
		SaveGameInstance->LatestLevelName = LatestLevelName;

		if (UnlockedDoors.Num() > 0)
		{
			SaveGameInstance->UnlockedDoors = UnlockedDoors;
		}

		SaveGameInstance->OpenedDoors = OpenedDoors;
		
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, "SaveSlot0", 0);
	}
}

void ASlashCharacter::LoadSave()
{
	USlashSaveGame* LoadedGame = Cast<USlashSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveSlot0", 0));
	SetAttributesFromSave(LoadedGame);
	if (!LoadedGame->EquippedWeaponId.IsEmpty())
	{
		TSubclassOf<AWeapon> SavedWeapon = GetSavedWeaponClassIfEquipable(LoadedGame->EquippedWeaponId);
		SpawnAndEquipWeapon(SavedWeapon);
	}
	else 
	{
		EquippedWeapon = nullptr;
		CharacterState = ECharacterState::ECS_Unequipped;
	}

	/*if (LoadedGame->LatestTeleportTransform.IsValid())
	{
		LatestTeleportTransform = LoadedGame->LatestTeleportTransform;
		TeleportTo(LatestTeleportTransform.GetLocation(), LatestTeleportTransform.Rotator());
	}*/

	if (LoadedGame->UnlockedDoors.Num() > 0)
	{
		UnlockedDoors = LoadedGame->UnlockedDoors;
	}

	OpenedDoors = LoadedGame->OpenedDoors;
	GetWorldTimerManager().SetTimer(LoadDoorsTimer, this, &ASlashCharacter::LoadOpenedDoors, 0.1f);
}

void ASlashCharacter::LoadOpenedDoors()
{
	TArray<AUnlockableDoor*> UnlockableDoors;
	TArray<AActor*> OutActors;

	UGameplayStatics::GetAllActorsOfClass(this, AUnlockableDoor::StaticClass(), OutActors);
	for (AActor* DoorActor : OutActors)
	{
		UnlockableDoors.Add(Cast<AUnlockableDoor>(DoorActor));
	}
	for (AUnlockableDoor* UnlockableDoor : UnlockableDoors)
	{
		if (OpenedDoors.Contains(UnlockableDoor->DoorId))
		{
			UnlockableDoor->DoorOpened();
		}
	}
}

void ASlashCharacter::SpawnAndEquipWeapon(TSubclassOf<AWeapon>& SavedWeapon)
{
	UWorld* World = GetWorld();
	if (World && SavedWeapon)
	{
		AWeapon* LoadedWeapon = World->SpawnActor<AWeapon>(SavedWeapon);
		EquipWeapon(LoadedWeapon);
		EquippedWeapon = LoadedWeapon;
	}
}

TSubclassOf<AWeapon> ASlashCharacter::GetSavedWeaponClassIfEquipable(FString WeaponId)
{
	TSubclassOf<AWeapon> SavedWeapon;
	for (TSubclassOf<AWeapon> EquipableWeapon : EquipableWeapons)
	{
		if (AWeapon* Weapon = Cast<AWeapon>(EquipableWeapon.GetDefaultObject()))
		{
			if (Weapon->GetWeaponId() == WeaponId)
			{
				SavedWeapon = EquipableWeapon;
			}
		}
	}
	return SavedWeapon;
}

void ASlashCharacter::SetAttributesFromSave(USlashSaveGame* LoadedGame)
{
	if (Attributes)
	{
		Attributes->SetHealth(LoadedGame->Health);
		Attributes->SetMaxHealth(LoadedGame->MaxHealth);
		Attributes->SetMaxStamina(LoadedGame->MaxStamina);
		Attributes->SetGold(LoadedGame->Gold);
		Attributes->SetSouls(LoadedGame->Souls);
		Attributes->SetLevel(LoadedGame->Level);
	}
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (IsOccupied()) return;

	const FVector2d MovementVector = Value.Get<FVector2d>();

	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ASlashCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

void ASlashCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingObject);
	ADoor* OverlappingDoor = Cast<ADoor>(OverlappingObject);
	AUnlockableDoor* OverlappingUnlockableDoor = Cast<AUnlockableDoor>(OverlappingObject);
	ASavePoint* OverlappingSavePoint = Cast<ASavePoint>(OverlappingObject);
	AInteractableKey* OverlappingKey = Cast<AInteractableKey>(OverlappingObject);
	if (OverlappingWeapon)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		EquipWeapon(OverlappingWeapon);
	}
	else if (OverlappingSavePoint && Attributes)
	{
		InteractWithSavePoint(OverlappingSavePoint);
	}
	else if (OverlappingUnlockableDoor)
	{
		InteractWithUnlockableDoor(OverlappingUnlockableDoor);
	}
	else if (OverlappingDoor && Attributes)
	{
		InteractWithDoor(OverlappingDoor);
	}
	else if (OverlappingKey)
	{
		InteractWithKey(OverlappingKey);

	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

void ASlashCharacter::Pause()
{
	if (PauseWidgetClass && !PauseWidget)
	{
		PauseWidget = CreateWidget<UPauseWidget>(GetWorld(), PauseWidgetClass);
		if (PauseWidget)
		{
			PauseWidget->AddToViewport();
		}
	}
}

void ASlashCharacter::Attack()
{
	if (CombatTarget)
	{
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();
		if (DistanceToTarget > CombatRadius)
		{
			CombatTarget = nullptr;
		}
	}
	Super::Attack();
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::Dodge()
{
	if (CanDodge())
	{
		Super::Dodge();
		ActionState = EActionState::EAS_Dodge;
		Attributes->UseStamina(Attributes->GetDodgeCost());
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void ASlashCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), WeaponSocket, this, this);
	OverlappingObject = nullptr;
	EquippedWeapon = Weapon;
	if (Weapon->GetWeaponType() == EWeaponType::EWT_OneHanded)
	{
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
	if (Weapon->GetWeaponType() == EWeaponType::EWT_TwoHanded)
	{
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	}
}

void ASlashCharacter::AttackEnd()
{
	Super::AttackEnd();
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanDodge()
{
	return Attributes && SlashOverlay &&
		IsUnoccupied() &&
		HasEnoughStamina(Attributes->GetDodgeCost());
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void ASlashCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_OneHanded)
	{
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_TwoHanded)
	{
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	}

	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);

	}
}

void ASlashCharacter::Die_Implementation()
{
	Super::Die_Implementation();

	ActionState = EActionState::EAS_Dead;
	CameraFadeOut(4.f);
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASlashCharacter::Respawn, 3.99f);
}

void ASlashCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), SheathSocket);
	}
}

void ASlashCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_OneHanded)
		{
			EquippedWeapon->AttachMeshToSocket(GetMesh(), WeaponSocket);
		}
		if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_TwoHanded)
		{
			EquippedWeapon->AttachMeshToSocket(GetMesh(), WeaponSocket);
		}
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::IsDodging()
{
	return ActionState == EActionState::EAS_Dodge;
}

bool ASlashCharacter::HasEnoughStamina(float staminaCost)
{
	return Attributes && Attributes->GetStamina() > staminaCost;
}

void ASlashCharacter::InitializeInputMapping(APlayerController* PlayerController)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(SlashContext, 0);
	}
}

void ASlashCharacter::InitializeSlashOverlay(APlayerController* PlayerController)
{
	ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
	if (SlashHUD)
	{
		SlashOverlay = SlashHUD->GetSlashOverlay();
		if (SlashOverlay && Attributes)
		{
			SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
			SlashOverlay->SetStaminaBarPercent(1.f);
			SlashOverlay->SetGold(Attributes->GetGold());
			SlashOverlay->SetSouls(Attributes->GetSouls());
			SlashOverlay->SetLevel(Attributes->GetLevel());
		}
	}
}

void ASlashCharacter::InitializeCameraManager()
{
	if (GetWorld())
	{
		CameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	}
}

void ASlashCharacter::CameraFadeIn(float duration)
{
	if (CameraManager)
	{
		CameraManager->StartCameraFade(1.f, 0.f, duration, FLinearColor(0.f, 0.f, 0.f, 1.f), true, false);
	}
}

void ASlashCharacter::CameraFadeIn1sec()
{
	if (CameraManager)
	{
		CameraManager->StartCameraFade(1.f, 0.f, 1.f, FLinearColor(0.f, 0.f, 0.f, 1.f), true, false);
	}
}

void ASlashCharacter::CameraFadeOut(float duration)
{
	if (CameraManager)
	{
		CameraManager->StartCameraFade(0.f, 1.f, duration, FLinearColor(0.f, 0.f, 0.f, 1.f), true, true);
	}
}

void ASlashCharacter::SetHUDHealth()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void ASlashCharacter::Respawn()
{
	LoadSave();
	Attributes->SetHealth(Attributes->GetMaxHealth());
	SaveGame();
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (!CurrentLevelName.IsEmpty())
	{
		UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
	}
}

void ASlashCharacter::InteractWithSavePoint(ASavePoint* OverlappingSavePoint)
{
	LatestLevelName = UGameplayStatics::GetCurrentLevelName(this);
	LatestTeleportTransform = GetActorTransform();
	Attributes->SetHealth(Attributes->GetMaxHealth());
	SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	SaveGame();
	OverlappingSavePoint->Interact();
	OverlappingSavePoint->ShowSaveWidget();
	CameraFadeOut(1.f);
	GetWorldTimerManager().SetTimer(SavingTimer, this, &ASlashCharacter::CameraFadeIn1sec, 0.9f);
}

void ASlashCharacter::InteractWithUnlockableDoor(AUnlockableDoor* OverlappingUnlockableDoor)
{
	if (UnlockedDoors.Contains(OverlappingUnlockableDoor->DoorId))
	{
		OverlappingUnlockableDoor->Interact();
		OpenedDoors.Add(OverlappingUnlockableDoor->DoorId);
	}
	else
	{
		OverlappingUnlockableDoor->ShowLockedWidget();
	}
}

void ASlashCharacter::InteractWithDoor(ADoor* OverlappingDoor)
{
	if (!OverlappingDoor->GetLevelName().IsEmpty() && OverlappingDoor->GetLevelStartTransform().IsValid())
	{
		LatestLevelName = OverlappingDoor->GetLevelName();
		LatestTeleportTransform = OverlappingDoor->GetLevelStartTransform();
		SaveGame();
		OverlappingDoor->Interact();
		CameraFadeOut(2.f);
	}
}

void ASlashCharacter::InteractWithKey(AInteractableKey* OverlappingKey)
{
	if (!OverlappingKey->GetLevelName().IsEmpty() && OverlappingKey->GetLevelStartTransform().IsValid())
	{
		LatestLevelName = OverlappingKey->GetLevelName();
		LatestTeleportTransform = OverlappingKey->GetLevelStartTransform();
		Attributes->SetHealth(Attributes->GetMaxHealth());
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
	UnlockedDoors.Add(OverlappingKey->UnlockedDoor);
	SaveGame();
	OverlappingKey->Interact();
}

