// Fill out your copyright notice in the Description page of Project Settings.

#include "BarrierHelper.h"
#include "Constants.h"

using namespace std;

const int Constants::MaxRegenerationCount = 1000;
const int Constants::SingleTry = 1;
const int Constants::PixelsShift = 300;
const int Constants::FieldLength = 984;

FVector UBarrierHelper::GetRandomLocation() {
	int HalfFieldLength = Constants::FieldLength / 2;

	int XCoord = FMath::RandRange(-HalfFieldLength, HalfFieldLength);
	int YCoord = FMath::RandRange(-HalfFieldLength, HalfFieldLength);

	return FVector(XCoord, YCoord, 0);
}

tuple<bool, FVector> UBarrierHelper::GetShiftedSpawnLocation(EMovementDirection LastMoveDirection, FVector SnakeHead) {
	tuple<bool, FVector> Coordinates;
	int HalfFieldLength = Constants::FieldLength / 2;
	float ShiftedCoord;
	FVector ShiftedLocation;

	switch (LastMoveDirection) {
	case EMovementDirection::RIGHT:
		ShiftedCoord = SnakeHead.X + Constants::PixelsShift;
		ShiftedLocation = FVector(ShiftedCoord, SnakeHead.Y, SnakeHead.Z);
		Coordinates = make_tuple(ShiftedCoord > HalfFieldLength, ShiftedLocation);
		break;
	case EMovementDirection::LEFT:
		ShiftedCoord = SnakeHead.X - Constants::PixelsShift;
		ShiftedLocation = FVector(ShiftedCoord, SnakeHead.Y, SnakeHead.Z);
		Coordinates = make_tuple(ShiftedCoord > -HalfFieldLength, ShiftedLocation);
		break;
	case EMovementDirection::UP:
		ShiftedCoord = SnakeHead.Y + Constants::PixelsShift;
		ShiftedLocation = FVector(SnakeHead.X, ShiftedCoord, SnakeHead.Z);
		Coordinates = make_tuple(ShiftedCoord > HalfFieldLength, ShiftedLocation);
		break;
	case EMovementDirection::DOWN:
		ShiftedCoord = SnakeHead.Y - Constants::PixelsShift;
		ShiftedLocation = FVector(SnakeHead.X, ShiftedCoord, SnakeHead.Z);
		Coordinates = make_tuple(ShiftedCoord > -HalfFieldLength, ShiftedLocation);
		break;
	}

	return Coordinates;
}

vector<BarrierPosition> UBarrierHelper::HandleBarrier(UWorld* CurrentWorld, EMovementDirection LastMoveDirection, FVector SnakeHead) {
	World = CurrentWorld;

	vector<BarrierPosition> Positions;
	BarrierType SelectedBarrier = static_cast<BarrierType>(rand() % BarrierTypeMax);
	tuple<bool, FVector> ShiftedLocation = GetShiftedSpawnLocation(LastMoveDirection, SnakeHead);
	bool IsValidLocation = !get<0>(ShiftedLocation);
	if (IsValidLocation) {
		Positions = GetPositions(SelectedBarrier, Constants::SingleTry, get<1>(ShiftedLocation));
		if (!Positions.empty()) {
			return Positions;
		}
	}

	FVector RandomLocation = GetRandomLocation();
	Positions = GetPositions(SelectedBarrier, Constants::MaxRegenerationCount, RandomLocation);

	return Positions;
}

vector<BarrierPosition> UBarrierHelper::GetPositions(BarrierType SelectedBarrier, int MaxRegenerationCount, FVector StartLocation) {
	vector<BarrierPosition> Positions;

	switch (SelectedBarrier) {
	case BarrierType::GForm:
		Positions = BarrierGForm(MaxRegenerationCount, StartLocation);
		break;
	case BarrierType::PForm:
		Positions = BarrierPForm(MaxRegenerationCount, StartLocation);
		break;
	case BarrierType::Cube:
		Positions = BarrierCube(MaxRegenerationCount, StartLocation);
		break;
	}

	return Positions;
}

vector<BarrierPosition> UBarrierHelper::BarrierGForm(int MaxRegenerationCount, FVector StartLocation) {
	vector<BarrierPosition> Positions;
	int Tries = 0;

	while (Tries < MaxRegenerationCount) {
		FVector Location1 = StartLocation;
		FVector BoxSize1 = FVector(40.0f, 100.0f, 50.0f);
		if (!IsLocationFree(Location1, BoxSize1)) {
			Tries++;
			continue;
		}

		FVector Location2 = Location1 + FVector(-50, 50, 0);
		FVector BoxSize2 = FVector(100.0f, 40.0f, 50.0f);
		if (!IsLocationFree(Location2, BoxSize2)) {
			Tries++;
			continue;
		}

		Positions.push_back({ Location1, FRotator(0, 0, 0) });
		Positions.push_back({ Location2, FRotator(0, 90, 0) });

		break;
	}

	return Positions;
}

vector<BarrierPosition> UBarrierHelper::BarrierPForm(int MaxRegenerationCount, FVector StartLocation) {
	vector<BarrierPosition> Positions;
	int Tries = 0;

	while (Tries < MaxRegenerationCount) {
		FVector Location1 = StartLocation;
		FVector BoxSize1 = FVector(40.0f, 100.0f, 50.0f);
		if (!IsLocationFree(Location1, BoxSize1)) {
			Tries++;
			continue;
		}

		FVector Location2 = Location1 + FVector(-50, 50, 0);
		FVector BoxSize2 = FVector(100.0f, 40.0f, 50.0f);
		if (!IsLocationFree(Location2, BoxSize2)) {
			Tries++;
			continue;
		}

		FVector Location3 = Location1 + FVector(-115, 0, 0);
		FVector BoxSize3 = FVector(40.0f, 100.0f, 50.0f);
		if (!IsLocationFree(Location3, BoxSize3)) {
			Tries++;
			continue;
		}

		Positions.push_back({ Location1, FRotator(0, 0, 0) });
		Positions.push_back({ Location2, FRotator(0, 90, 0) });
		Positions.push_back({ Location3, FRotator(0, 0, 0) });

		break;
	}

	return Positions;
}

vector<BarrierPosition> UBarrierHelper::BarrierCube(int MaxRegenerationCount, FVector StartLocation) {
	vector<BarrierPosition> Positions;
	int Tries = 0;
	while (Tries < MaxRegenerationCount) {
		FVector Location = StartLocation;
		FVector BoxSize = FVector(100.0f, 100.0f, 50.0f);
		FVector Scale = FVector(1.0f, 1.0f, 1.0f);
		if (!IsLocationFree(Location, BoxSize)) {
			Tries++;
			continue;
		}
		
		Positions.push_back({ Location, FRotator(0, 0, 0), Scale });

		break;
	}

	return Positions;
}

bool UBarrierHelper::IsLocationFree(FVector Location, FVector BoxSize) {
	FCollisionShape Shape = FCollisionShape::MakeBox(BoxSize);
	return !World->OverlapAnyTestByChannel(Location, FQuat::Identity, ECollisionChannel::ECC_WorldStatic, Shape);
}

bool UBarrierHelper::IsImaginaryBarrier() {
	return rand() % 2 == 0;
}