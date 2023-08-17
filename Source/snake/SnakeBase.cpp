// Fill out your copyright notice in the Description page of Project Settings.

#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include <vector>

const float COUNT_OF_FPS = 30.f;

FVector MovementVector;

void UseDefaultVectorValues(FVector& vector) {
	vector.X = 0;
	vector.Y = 0;
	vector.Z = 0;
}

// Sets default values
ASnakeBase::ASnakeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 1.f;
	LastMoveDirection = EMovementDirection::UP;
	UseDefaultVectorValues(MovementVector);
}
void LogPrint(FVector MovementVector1);
// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(1.f / COUNT_OF_FPS);
	AddSnakeElement(10);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
	UseDefaultVectorValues(MovementVector);
}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; i++) {

		FVector NewLocation(0, ElementSize * i, 0);
		FTransform NewTrasform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTrasform);
		int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
		if (ElemIndex == 0) {
			NewSnakeElem->SetFirstElementType();
		}
	}
}

void LogPrint(FVector PrevLocation) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		FString::Printf(TEXT("X: %f, Y: %f, Z: %f"), PrevLocation.X, PrevLocation.Y, PrevLocation.Z));
}

FVector ShiftSnakeHead(TArray<ASnakeElementBase*>& SnakeElements, float MovementSpeed, float ElementSize, EMovementDirection LastMoveDirection, FVector MovementVector1) {
	float shift = MovementSpeed / COUNT_OF_FPS * ElementSize;
	switch (LastMoveDirection) {
	case EMovementDirection::UP:
		MovementVector1.Y += shift;
		break;
	case EMovementDirection::DOWN:
		MovementVector1.Y -= shift;
		break;
	case EMovementDirection::LEFT:
		MovementVector1.X -= shift;
		break;
	case EMovementDirection::RIGHT:
		MovementVector1.X += shift;
		break;
	}

	return MovementVector1;
}

FVector RecalculateTail(FVector PrevLocation, EMovementDirection LastMoveDirection, float ElementSize) {
	switch (LastMoveDirection) {
	case EMovementDirection::UP:
		PrevLocation.Y -= ElementSize;
		break;
	case EMovementDirection::DOWN:
		PrevLocation.Y += ElementSize;
		break;
	case EMovementDirection::LEFT:
		PrevLocation.X += ElementSize;
		break;
	case EMovementDirection::RIGHT:
		PrevLocation.X -= ElementSize;
		break;
	}

	return PrevLocation;
}

void ShiftSnakeTail(TArray<ASnakeElementBase*>& SnakeElements, EMovementDirection LastMoveDirection, float ElementSize) {
	FVector PrevLocation = SnakeElements[0]->GetActorLocation();
	for (int i = 1; i < SnakeElements.Num(); i++) {
		FVector CurrentLocation = SnakeElements[i]->GetActorLocation();
		FVector NewLocation = RecalculateTail(PrevLocation, LastMoveDirection, ElementSize);
		SnakeElements[i]->SetActorLocation(NewLocation);
		PrevLocation = CurrentLocation;
	}
}

void ASnakeBase::Move()
{
	ShiftSnakeTail(SnakeElements, LastMoveDirection, ElementSize);
	FVector NewHeadLocation = ShiftSnakeHead(SnakeElements, MovementSpeed, ElementSize, LastMoveDirection, MovementVector);;
	SnakeElements[0]->AddActorWorldOffset(NewHeadLocation);
	void LogPrint(FVector PrevLocation);

	ASnakeBase::CanChangeHorizontalDirection = true;
	ASnakeBase::CanChangeVerticalDirection = true;
}
