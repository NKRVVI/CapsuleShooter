// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/HealthAttribute.h"

// Sets default values for this component's properties
UHealthAttribute::UHealthAttribute()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthAttribute::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthAttribute::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthAttribute::DecrementHealth(float Decrement)
{
	Health = FMath::Clamp(Health - Decrement, 0.f, MaxHealth); 
	//UE_LOG(LogTemp, Warning, TEXT("Health is %f"), Health);
}

