// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CapsulePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ACapsulePlayer;

UCLASS()
class CAPSULESHOOTER_API ACapsulePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACapsulePlayerController();

	virtual void Tick(float DeltaTime) override;

	virtual void DisableInput(class APlayerController* PlayerController) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	void Move(const FInputActionValue& InputActionValue);
	void Shoot(const FInputActionValue& InputActionValue);

	ACapsulePlayer* CapsuleShooterPlayer;

	UPROPERTY(EditDefaultsOnly)
	float RotationInterpSpeed = 10;

	UPROPERTY(BlueprintReadOnly)
	FVector HitLocation;

	UPROPERTY(BlueprintReadOnly)
	FVector2D CursorLocation;

	bool bShouldMove = true;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShootInputAction;

public:
	FORCEINLINE FVector GetHitLocation() const { return HitLocation; }
};
