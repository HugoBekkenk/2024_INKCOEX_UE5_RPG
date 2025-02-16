// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "SlashCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class USlashOverlay;
class USlashSaveGame;
class ASavePoint;
class AInteractableObject;
class AUnlockableDoor;
class USoundBase;
class UPauseWidget;
class AInteractableKey;

UCLASS()
class MYPROJECT_API ASlashCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
	virtual void AddHealth(APotion* Potion) override;
	virtual void AddStamina(APotion* Potion) override;
	void CameraFadeIn(float duration);
	void CameraFadeIn1sec();
	void CameraFadeOut(float duration);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AInteractableObject> OverlappingObject;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPauseWidget> PauseWidgetClass;

	UPROPERTY()
	UPauseWidget* PauseWidget;

protected:
	virtual void BeginPlay() override;

	void InitializeSave();

	void InitializeCharacter();

	void SaveGame();
	void LoadSave();
	void LoadOpenedDoors();
	void SpawnAndEquipWeapon(TSubclassOf<AWeapon>& SavedWeapon);
	TSubclassOf<AWeapon> GetSavedWeaponClassIfEquipable(FString WeaponId);
	void SetAttributesFromSave(USlashSaveGame* LoadedGame);

	/** Callback for input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void Pause();
	virtual void Attack() override;
	virtual void Dodge() override;

	/** Combat */
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;
	virtual bool CanDodge() override;
	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();
	void PlayEquipMontage(const FName& SectionName);
	virtual void Die_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	/** Action Binding */
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> SlashContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> EKeyAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LevelUpSound;

private:
	bool IsUnoccupied();
	bool IsOccupied();
	bool IsDodging();
	bool HasEnoughStamina(float StaminaCost);
	void InitializeInputMapping(APlayerController* PlayerController);
	void InitializeSlashOverlay(APlayerController* PlayerController);
	void InitializeCameraManager();
	void SetHUDHealth();
	void Respawn();
	void InteractWithSavePoint(ASavePoint* OverlappingSavePoint);
	void InteractWithUnlockableDoor(AUnlockableDoor* OverlappingUnlockableDoor);
	void InteractWithDoor(ADoor* OverlappingDoor);
	void InteractWithKey(AInteractableKey* OverlappingKey);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ViewCamera;

	UPROPERTY(VisibleAnywhere, Category=Hair)
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	TObjectPtr<UGroomComponent> Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> EquipMontage;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditAnywhere, Category = Combat)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = Sockets)
	FName WeaponSocket = FName("RightHandSocket");

	UPROPERTY(EditAnywhere, Category = Sockets)
	FName SheathSocket = FName("SpineSocket");

	UPROPERTY(EditAnywhere)
	FName ActorTag = FName("EngageableTarget");

	UPROPERTY()
	TObjectPtr<USlashOverlay> SlashOverlay;

	UPROPERTY()
	APlayerCameraManager* CameraManager;

	FTransform LatestTeleportTransform;

	FString LatestLevelName;

	TArray<FString> UnlockedDoors;
	TArray<FString> OpenedDoors;

	FTimerHandle RespawnTimer;
	FTimerHandle SavingTimer;
	FTimerHandle LoadDoorsTimer;


public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
