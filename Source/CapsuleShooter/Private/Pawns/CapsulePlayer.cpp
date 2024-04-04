// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/CapsulePlayer.h"
#include "Components/CapsuleComponent.h"
#include "Items/Gun.h"
#include "Attributes/HealthAttribute.h"
#include "HUD/HealthBarWidget.h"
#include "Controller/CapsulePlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Controller/CapsulePlayerController.h"

// Sets default values
ACapsulePlayer::ACapsulePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->SetupAttachment(GetRootComponent());

	GunAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GunAttachPoint"));
	GunAttachPoint->SetupAttachment(GetRootComponent());

	HealthAttribute = CreateDefaultSubobject<UHealthAttribute>(TEXT("Health"));

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
}

void ACapsulePlayer::Shoot()
{
	if (Gun) Gun->Shoot();
}

FVector ACapsulePlayer::GetVolumeDimensions()
{
	return ItemMesh->GetStaticMesh()->GetBoundingBox().GetExtent();
}

void ACapsulePlayer::DestroySelf()
{
	Gun->Destroy();
	Destroy();
}

float ACapsulePlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//UE_LOG(LogTemp, Warning, TEXT("Damage amount is %f"), DamageAmount);
	GetDamaged(DamageAmount);
	UpdateHealthBar();
	if (GetHealth() <= 0.f)
	{
		Die();
	}
	return DamageAmount;
}

void ACapsulePlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PlayerController = Cast<ACapsulePlayerController>(NewController);
}

// Called when the game starts or when spawned
void ACapsulePlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ACapsulePlayer::GetDamaged(float Damage)
{
	if (HealthAttribute)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Decrementing health"));
		HealthAttribute->DecrementHealth(Damage);
	}
}

void ACapsulePlayer::SetHealth(float _Health)
{
	if (HealthAttribute)
	{
		HealthAttribute->SetHealth(_Health);
		UpdateHealthBar();
	}
}

void ACapsulePlayer::SetMaxHealth(float _MaxHealth)
{
	if (HealthAttribute)
	{
		HealthAttribute->SetMaxHealth(_MaxHealth);
		UpdateHealthBar();
	}
}

float ACapsulePlayer::GetWidth()
{
	return CapsuleCollision->GetScaledCapsuleRadius();
}

float ACapsulePlayer::GetHealth()
{
	if (HealthAttribute) return HealthAttribute->GetHealth();
	return 0;
}

float ACapsulePlayer::GetMaxHealth()
{
	if (HealthAttribute) return HealthAttribute->GetMaxHealth();
	return 0;
}

void ACapsulePlayer::UpdateHealthBar()
{
	if (HealthBar && HealthAttribute)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Updating HUD"));
		HealthBar->SetHealthPercent(HealthAttribute->GetPercent());
	}
}

void ACapsulePlayer::Die()
{
	PlayerDiedDelegate.Broadcast();
	Cast<ACapsulePlayerController>(GetController())->DisableInput(nullptr);
}

// Called every frame
void ACapsulePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));

	PointGunToTarget();

	if (GetActorLocation().Z <= -1000.f && !bAnnouncedDeath)
	{
		Die();
		bAnnouncedDeath = true;
	}
}

// Called to bind functionality to input
void ACapsulePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACapsulePlayer::SetGunClass(TSubclassOf<AGun> _GunClass)
{
	GunClass = _GunClass;

	if (UWorld* World = GetWorld())
	{
		Gun = World->SpawnActor<AGun>(GunClass, GunAttachPoint->GetComponentTransform());
		FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
		//FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, false);
		Gun->AttachToActor(this, TransformRules);
		
		PointGunToTarget();
	}
}

void ACapsulePlayer::PointGunToTarget()
{
	if (PlayerController && Gun)
	{
		FVector TargetLocation = PlayerController->GetHitLocation();
		TargetLocation.Z = Gun->GetActorLocation().Z;
		if ((Gun->GetActorLocation() - TargetLocation).Size() <= Gun->GetRotationRadius()) return;
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Gun->GetActorLocation(), TargetLocation);
		LookAtRotation.Pitch = Gun->GetActorRotation().Pitch;
		LookAtRotation.Roll = Gun->GetActorRotation().Roll;
		Gun->SetActorRotation(LookAtRotation);
	}
}

