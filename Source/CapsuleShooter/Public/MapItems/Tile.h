// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class CAPSULESHOOTER_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	void StartSpawn();
	void StopSpawn();
	void SetObstacleColour(FLinearColor ObstacleColour);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Flash(bool bIsColourToFlash);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* Tile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Obstacle;
	
	UPROPERTY(EditDefaultsOnly);
	UStaticMesh* ObstacleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsObstacle = false;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor NormalColour;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor FlashColour;

	UPROPERTY(EditDefaultsOnly)
	float FlashInterpSpeed;

	float FlashAlpha = 0.f;

	bool bIsSpawning = false;

	FTimerHandle FlashTimer;

	UMaterialInstanceDynamic* TileDynamicMaterial;
	UMaterialInstanceDynamic* ObstacleDynamicMaterial;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetTileBounds();

	//sets the obstacle static mesh component to a selected obstacle mesh
	void BecomeObstacle() { if (ObstacleMesh) Obstacle->SetStaticMesh(ObstacleMesh); bIsObstacle = true; }
	void UnbecomeObstacle() { Obstacle->SetStaticMesh(nullptr); bIsObstacle = false; }
	FORCEINLINE bool IsObstacle() { return bIsObstacle; }
};
