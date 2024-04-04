// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class CAPSULESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
	
	void Shoot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere)
	TArray<USceneComponent*> BulletSpawnPoints;

	UPROPERTY(VisibleAnywhere)
	bool bCanShoot = true;

	UPROPERTY(EditAnywhere)
	float SecondsPerShot;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABullet> BulletClass;

	FTimerHandle ShootingTimer;

	UWorld* World;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* GunSound;

	UPROPERTY(EditDefaultsOnly)
	float RotationRadius;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USceneComponent* MuzzleFlashPoint;

	float time = 2.f;
	float current = 0.f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE float GetRotationRadius() const { return RotationRadius; }
};
