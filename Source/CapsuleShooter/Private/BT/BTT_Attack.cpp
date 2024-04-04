// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTT_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawns/CapsulePlayer.h"
#include "Pawns/Enemy.h"
#include "Kismet/GameplayStatics.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = "Attack";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	Self = Cast<AEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));
	Target = Cast<ACapsulePlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!Target) return EBTNodeResult::Succeeded;
	AttackSpeed = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AttackSpeedKey.SelectedKeyName);
	Damage = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(DamageKey.SelectedKeyName);

	SelfLocation = Self->GetActorLocation();
	TargetLocation = Target->GetActorLocation();

	TowardsTarget = true;

	FVector TargetDirection = (TargetLocation - SelfLocation).GetSafeNormal();

	TargetLocation -= TargetDirection * Target->GetWidth() * 2;

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsAttackingKey.SelectedKeyName, true);

	if (SelfLocation.Equals(TargetLocation))
	{
		UGameplayStatics::ApplyDamage(Target, Damage, Self->GetController(), Self, UDamageType::StaticClass());
		Self->PlayAttackSound();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UBTT_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FVector InterpLocation;
	if (TowardsTarget)
	{
		InterpLocation = FMath::VInterpConstantTo(Self->GetActorLocation(), TargetLocation, DeltaSeconds, AttackSpeed);
	}
	else
	{
		InterpLocation = FMath::VInterpConstantTo(Self->GetActorLocation(), SelfLocation, DeltaSeconds, AttackSpeed);
	}

	Self->SetActorLocation(InterpLocation);

	//UE_LOG(LogTemp, Warning, TEXT("Self location: %s | TargetLocation: %s | InterpLocation: %s"), *SelfLocation.ToString(), *TargetLocation.ToString(),
		//*InterpLocation.ToString());

	//UE_LOG(LogTemp, Warning, TEXT("%d"), ++NumLoops);

	FVector TargetDirection = (Target->GetActorLocation() - Self->GetActorLocation()).GetSafeNormal();
	if (Self->GetActorLocation().Equals(Target->GetActorLocation() - TargetDirection * Target->GetWidth() * 2, 1))
	{
		TowardsTarget = false;
		UGameplayStatics::ApplyDamage(Target, Damage, Self->GetController(), Self, UDamageType::StaticClass());
		Self->PlayAttackSound();
	}

	if (Self->GetActorLocation().Equals(TargetLocation, 1) && TowardsTarget)
	{
		TowardsTarget = false;
	}
	else if (Self->GetActorLocation().Equals(SelfLocation, 1) && !TowardsTarget)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsAttackingKey.SelectedKeyName, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}