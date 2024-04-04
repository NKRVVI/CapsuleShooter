// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Controller/CapsuleEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Sound/SoundCue.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->SetupAttachment(GetRootComponent());
}

void AEnemy::Die()
{
	EnemyDeadDelegate.Broadcast();
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DeathNiagaraSystem, GetActorLocation() + FVector(0, 0, CapsuleCollision->GetScaledCapsuleHalfHeight()), FRotator(0, 0, 0));
	if(DeathSound) UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	Destroy();
}

void AEnemy::PlayAttackSound()
{
	if (AttackSound) UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//GetWorldTimerManager().SetTimer(DeathTimerHandle,this, &ThisClass::Die, 10.f);
}

void AEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	Controller = Cast<ACapsuleEnemyController>(NewController);

	Blackboard = Controller->GetBlackboardComponent();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AEnemy::IsAttacking()
{
	return Blackboard->GetValueAsBool("IsAttacking");
}

