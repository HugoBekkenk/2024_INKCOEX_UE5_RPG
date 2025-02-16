// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);

protected:
	virtual void BeginPlay() override;

private:
	// Current Health
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

	// Current Stamina
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold = 0;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls = 0;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 14.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate = 8.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Level = 1;

public:
	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);
	bool LevelUp();
	float GetHealthPercent();
	float GetStaminaPercent();
	bool IsAlive();
	void AddGold(int32 AmountOfGold);
	void AddSouls(int32 NumberOfSouls);
	void AddHealth(int32 HealedAmount);
	void AddStamina(int32 RegeneratedAmount);
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetGold() const { return Gold; }

	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE int32 GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE int32 GetLevel() const { return Level; }

	FORCEINLINE void SetHealth(float NewHealth) { Health = NewHealth; }
	FORCEINLINE void SetMaxHealth(float NewMaxHealth) { MaxHealth = NewMaxHealth; }
	FORCEINLINE void SetStamina(float NewStamina) { Stamina = NewStamina; }
	FORCEINLINE void SetMaxStamina(float NewMaxStamina) { MaxStamina = NewMaxStamina; }
	FORCEINLINE void SetGold(int32 NewGold) { Gold = NewGold; }
	FORCEINLINE void SetSouls(int32 NewSouls) { Souls = NewSouls; }
	FORCEINLINE void SetLevel(int32 NewLevel) { Level = NewLevel; }
};
