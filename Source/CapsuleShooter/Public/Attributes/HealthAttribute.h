// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthAttribute.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAPSULESHOOTER_API UHealthAttribute : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthAttribute();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DecrementHealth(float Decrement);
	void SetHealth(float _Health) { Health = FMath::Clamp(_Health, 0.f, MaxHealth); }
	void SetMaxHealth(float _MaxHealth) { MaxHealth = _MaxHealth; Health = FMath::Clamp(Health, 0.f, MaxHealth); }
	float GetPercent() { return Health / MaxHealth; }
	float GetHealth() { return Health; }
	float GetMaxHealth() { return MaxHealth; }
};
