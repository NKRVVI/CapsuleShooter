#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Items/Gun.h"
#include "LevelStruct.generated.h"

USTRUCT(BlueprintType)
struct FLevelData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float SecondsBetweenSpawn;

	UPROPERTY(EditAnywhere)
	int32 NumTiles;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGun> GunClass;

	UPROPERTY(EditAnywhere)
	bool bInfiniteSpawn;

	UPROPERTY(EditAnywhere)
	int32 EnemiesToBeSpawned;

	UPROPERTY(EditAnywhere, meta = (UIMin = "0.0", UIMax = "1.0"))
	float ObstaclePercent;
};

USTRUCT (BlueprintType)
struct FColourPalette : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FLinearColor StartColour;

	UPROPERTY(EditAnywhere)
	FLinearColor EndColour;
};