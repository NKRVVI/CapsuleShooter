// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CapsulePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pawns/CapsulePlayer.h"
#include "HUD/CapsuleShooterHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/Enemy.h"

ACapsulePlayerController::ACapsulePlayerController()
{
	
}

void ACapsulePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bShouldMove) return;

	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, HitResult);
	HitLocation = HitResult.ImpactPoint;

	if (APawn* ControlledPawn = GetPawn())
	{
		FVector TargetLocation;
		if (AEnemy* Enemy = Cast<AEnemy>(HitResult.GetActor()))
		{
			TargetLocation = HitResult.GetActor()->GetActorLocation();
			//UE_LOG(LogTemp, Warning, TEXT("Cursor is on target"));
		}
		else
		{
			TargetLocation = HitResult.ImpactPoint;
		}
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), TargetLocation);

		FRotator TargetControlRotation = FMath::RInterpConstantTo(GetControlRotation(), LookAtRotation, DeltaTime, RotationInterpSpeed);

		SetControlRotation(LookAtRotation);

		HitLocation = TargetLocation;
		///////////////////////////////////////////////////////////////
		/*float CursorX;
		float CursorY;
		GetMousePosition(CursorX, CursorY);
		CursorLocation = FVector2D(CursorX, CursorY);
		UE_LOG(LogTemp, Warning, TEXT("Cursor locatin is %s"), *CursorLocation.ToString());
		FVector CrosshairWorldPosition, CrosshairWorldDirection;
		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(this, CursorLocation, CrosshairWorldPosition, CrosshairWorldDirection);

		LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), CrosshairWorldPosition);
		SetControlRotation(LookAtRotation);*/

	}
}

void ACapsulePlayerController::DisableInput(APlayerController* PlayerController)
{
	Super::DisableInput(PlayerController);

	bShouldMove = false;
}

void ACapsulePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	Subsystem->AddMappingContext(DefaultContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ACapsulePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(ShootInputAction, ETriggerEvent::Triggered, this, &ThisClass::Shoot);
	}
}

void ACapsulePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ACapsuleShooterHUD* HUD = GetHUD<ACapsuleShooterHUD>())
	{
		HUD->AddHealthBarWidgetToViewport();
		GetPawn<ACapsulePlayer>()->SetHealthBarWidget(HUD->GetHealthBarWidget());
	}
}

void ACapsulePlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(FVector(1, 0, 0), InputAxisVector.Y);
		ControlledPawn->AddMovementInput(FVector(0, 1, 0), InputAxisVector.X);
	}
}

void ACapsulePlayerController::Shoot(const FInputActionValue& InputActionValue)
{
	if (ACapsulePlayer* Capsule = Cast<ACapsulePlayer>(GetPawn()))
	{
		Capsule->Shoot();
	}
}
