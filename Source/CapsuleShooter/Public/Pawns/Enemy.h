// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FDead)

class UParticleSystem;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundCue;

UCLASS()
class CAPSULESHOOTER_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

	void Die();

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* BTAsset;

	FDead EnemyDeadDelegate;

	void PlayAttackSound();

	UPROPERTY(EditDefaultsOnly);
	USoundCue* DeathSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;


	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraSystem* DeathNiagaraSystem;

	UPROPERTY(EditDefaultsOnly)
	float AttackCooldown;

	UPROPERTY(EditDefaultsOnly)
	float AttackSpeed;

	UPROPERTY(EditDefaultsOnly)
	float AttackRadius;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

	class ACapsuleEnemyController* Controller;
	class UBlackboardComponent* Blackboard;

	FTimerHandle DeathTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* AttackSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE float GetAttackCooldown() { return AttackCooldown; }
	FORCEINLINE float GetAttackSpeed() { return AttackSpeed; }
	FORCEINLINE float GetAttackRadius() { return AttackRadius; }
	FORCEINLINE float GetDamage() { return Damage; }

	UFUNCTION(BlueprintCallable)
	bool IsAttacking();
};
