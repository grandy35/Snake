// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Models/BarrierPosition.h"
#include <vector>
#include "Barrier.h"
#include "BarrierHelper.generated.h"

UENUM()
enum BarrierType {
	GForm,
	PForm,
	Cube,

	BarrierTypeMax,
};

UCLASS()
class SNAKE_API UBarrierHelper : public UObject
{
	GENERATED_BODY()

public:
	std::vector<BarrierPosition> HandleBarrier(UWorld* CurrentWorld, EMovementDirection LastMoveDirection, FVector SnakeHead);
	bool IsImaginaryBarrier();

private:
	FVector GetRandomLocation();

	std::vector<BarrierPosition> BarrierGForm(int MaxRegenerationCount, FVector StartLocation);
	std::vector<BarrierPosition> BarrierPForm(int MaxRegenerationCount, FVector StartLocation);
	std::vector<BarrierPosition> BarrierCube(int MaxRegenerationCount, FVector StartLocation);

	std::tuple<bool, FVector> GetShiftedSpawnLocation(EMovementDirection LastMoveDirection, FVector SnakeHead);
	std::vector<BarrierPosition> GetPositions(BarrierType SelectedBarrier, int MaxRegenerationCount, FVector StartLocation);

	bool IsLocationFree(FVector Location, FVector BoxSize);

	UWorld* World;
};
