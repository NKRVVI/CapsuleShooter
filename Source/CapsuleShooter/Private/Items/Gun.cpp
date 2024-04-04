// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Gun.h"
#include "Gun/Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	SetRootComponent(GunMesh);

	MuzzleFlashPoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleFlash"));
	MuzzleFlashPoint->SetupAttachment(GetRootComponent());
}

void AGun::Shoot()
{
	if (!bCanShoot) return;
	if (World)
	{
		for (auto Component : BulletSpawnPoints)
		{
			if (Component)
			{
				World->SpawnActor<ABullet>(BulletClass, Component->GetComponentTransform());
			}
		}
		if (GunSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, GunSound, GetActorLocation());
		}

		if (MuzzleFlash)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Muzzle flash transform %s"), *MuzzleFlashPoint->GetComponentTransform().ToString());
			UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlash, MuzzleFlashPoint->GetComponentTransform());
			//UGameplayStatics::SpawnEmitterAttached(Tracer, BulletCollision, FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition);
		}
	}
	
	bCanShoot = false;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]() {bCanShoot = true; });
	GetWorldTimerManager().SetTimer(ShootingTimer, TimerDelegate, SecondsPerShot, false);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	World = GetWorld();

	for (auto& Component : GetComponentsByTag(USceneComponent::StaticClass(), FName("BulletSpawnPoint")))
	{
		if (USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
		{
			BulletSpawnPoints.Add(SceneComponent);
		}
	}
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*current += DeltaTime;
	if (current >= time)
	{
		Shoot();
		current = 0.f;
	}*/
}

