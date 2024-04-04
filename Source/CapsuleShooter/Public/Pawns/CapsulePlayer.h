// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CapsulePlayer.generated.h"

DECLARE_MULTICAST_DELEGATE(FPlayerDiedDelegate)

class AGun;
class UHealthAttribute;
class UHealthBarWidget;

UCLASS()
class CAPSULESHOOTER_API ACapsulePlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACapsulePlayer();

	void Shoot();

	FVector GetVolumeDimensions();

	void DestroySelf();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void PossessedBy(AController* NewController) override;

	FPlayerDiedDelegate PlayerDiedDelegate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetDamaged(float Damage);
	void UpdateHealthBar();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* GunAttachPoint;

	TSubclassOf<AGun> GunClass;

	UPROPERTY(BlueprintReadOnly)
	AGun* Gun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UHealthAttribute* HealthAttribute;

	UHealthBarWidget* HealthBar;

	UPROPERTY()
	class ACapsulePlayerController* PlayerController;

	void Die();

	bool bAnnouncedDeath = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetGunClass(TSubclassOf<AGun> _GunClass);

	void PointGunToTarget();

	FORCEINLINE void SetHealthBarWidget(UHealthBarWidget* _HealthBarWidget) { HealthBar = _HealthBarWidget; }

	float GetHealth();
	float  GetMaxHealth();
	void SetHealth(float _Health);
	void SetMaxHealth(float _MaxHealth);
	float GetWidth();
};
