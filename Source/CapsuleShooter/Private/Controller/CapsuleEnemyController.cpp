// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CapsuleEnemyController.h"
#include "Pawns/Enemy.h"
#include "Pawns/CapsulePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameMode/CapsuleShooterGameMode.h"

/*
	onpossess is overrode to run the behaviour tree and to blackboard values are initialised
*/
void ACapsuleEnemyController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	PossessedEnemy = Cast<AEnemy>(GetPawn());

	if (PossessedEnemy && PossessedEnemy->BTAsset)
	{
		RunBehaviorTree(PossessedEnemy->BTAsset);
		GetBlackboardComponent()->SetValueAsObject("Player", Cast<ACapsulePlayer>(UGameplayStatics::GetPlayerPawn(this, 0)));
		GetBlackboardComponent()->SetValueAsObject("SelfActor", PossessedEnemy);
		GetBlackboardComponent()->SetValueAsFloat("AttackCooldown", PossessedEnemy->GetAttackCooldown());
		GetBlackboardComponent()->SetValueAsFloat("AttackSpeed", PossessedEnemy->GetAttackSpeed());
		GetBlackboardComponent()->SetValueAsFloat("AttackRadius", PossessedEnemy->GetAttackRadius());
		GetBlackboardComponent()->SetValueAsFloat("Damage", PossessedEnemy->GetDamage());
		bool ShouldMove = Cast<ACapsuleShooterGameMode>(GetWorld()->GetAuthGameMode())->bShouldMove;
		UE_LOG(LogTemp, Warning, TEXT("Should move set to %d"), ShouldMove);
		GetBlackboardComponent()->SetValueAsBool("ShouldMove", ShouldMove);
	}

	/*Cast<ACapsuleShooterGameMode>(UGameplayStatics::GetGameMode(this))->StopEnemies.AddLambda([this]() {
		UE_LOG(LogTemp, Warning, TEXT("Hello"));
		if(PossessedEnemy) PossessedEnemy->DeathSound = nullptr;
		});*/
}

void ACapsuleEnemyController::StopEnemyMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("ShouldMove set to false"));
	//GetBlackboardComponent()->SetValueAsBool("ShouldMove", false);
	
}
