// Fill out your copyright notice in the Description page of Project Settings.


#include "MapItems/Tile.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile"));
	SetRootComponent(Tile);

	Obstacle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Obstacle"));
	Obstacle->SetupAttachment(GetRootComponent());

}

void ATile::StartSpawn()
{
	bIsSpawning = true;
	Flash(true);
}

void ATile::StopSpawn()
{
	//UE_LOG(LogTemp, Warning, TEXT("Tile stop flashing"));
	bIsSpawning = false;
	TileDynamicMaterial->SetVectorParameterValue("Color", NormalColour);
	FlashAlpha = 0.f;
}

void ATile::SetObstacleColour(FLinearColor ObstacleColour)
{
	if (!bIsObstacle) return;

	auto Material = Obstacle->GetMaterial(0);
	ObstacleDynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	Obstacle->SetMaterial(0, ObstacleDynamicMaterial);

	ObstacleDynamicMaterial->SetVectorParameterValue("Color", ObstacleColour);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()

{
	Super::BeginPlay();

	auto Material = Tile->GetMaterial(0);
	TileDynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	Tile->SetMaterial(0, TileDynamicMaterial);
	TileDynamicMaterial->SetVectorParameterValue("Color", NormalColour);
}

void ATile::Flash(bool bIsColourToFlash)
{
	if (!bIsSpawning) return;
	
	FLinearColor InterpColour;

	if (bIsColourToFlash)
	{
		FlashAlpha = FMath::FInterpConstantTo(FlashAlpha, 1.f, GetWorld()->GetDeltaSeconds(), FlashInterpSpeed);
		InterpColour = FMath::Lerp<FLinearColor>(NormalColour, FlashColour, FlashAlpha);
		//UE_LOG(LogTemp, Warning, TEXT("Flash to red"));
	}
	else
	{
		FlashAlpha = FMath::FInterpConstantTo(FlashAlpha, 0.f, GetWorld()->GetDeltaSeconds(), FlashInterpSpeed);
		InterpColour = FMath::Lerp<FLinearColor>(NormalColour, FlashColour, FlashAlpha);
		//UE_LOG(LogTemp, Warning, TEXT("Flash to normal"));
	}
	TileDynamicMaterial->SetVectorParameterValue("Color", InterpColour);
	//UE_LOG(LogTemp, Warning, TEXT("Flash alpha is %f"), FlashAlpha);

	//FTimerDelegate FlashTimerDelegate = FTimerDelegate::CreateUObject(this, &ATile::Flash, bIsColourToFlash);
	FTimerDelegate FlashTimerDelegate;

	if (1.f - FlashAlpha < 0.05f && bIsColourToFlash)
	{
		FlashTimerDelegate = FTimerDelegate::CreateUObject(this, &ATile::Flash, false);
		//UE_LOG(LogTemp, Warning, TEXT("Switching to normal"));
	}
	else if (FlashAlpha < 0.05f && !bIsColourToFlash)
	{
		FlashTimerDelegate = FTimerDelegate::CreateUObject(this, &ATile::Flash, true);
		//UE_LOG(LogTemp, Warning, TEXT("Switching to red"));
	}
	else
	{
		FlashTimerDelegate = FTimerDelegate::CreateUObject(this, &ATile::Flash, bIsColourToFlash);
		//UE_LOG(LogTemp, Warning, TEXT("%d"), bIsColourToFlash);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Delta seconds is %f"), GetWorld()->GetDeltaSeconds());
	GetWorldTimerManager().SetTimer(FlashTimer, FlashTimerDelegate, 0.01, false);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ATile::GetTileBounds()
{
	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;
	UKismetSystemLibrary::GetComponentBounds(Tile, Origin, BoxExtent, SphereRadius);

	return BoxExtent;
}

