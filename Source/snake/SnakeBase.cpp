// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "IInteractable.h"

// Sets default values
ASnakeBase::ASnakeBase() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 0.3f;
	LastMoveDirection = EMovementDirection::UP;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay() {
	Super::BeginPlay();

	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(4);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	Move();
}

void ASnakeBase::AddSnakeElement(int ElementsNum) {
	for (int i = 0; i < ElementsNum; i++) {
		FVector NewLocation(0, SnakeElements.Num() * -ElementSize, 0);

		FTransform NewTrasform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTrasform);
		NewSnakeElem->SnakeOwner = this;
		int32 ElemIndex = SnakeElements.Add(NewSnakeElem);

		if (ElemIndex == 0) {
			NewSnakeElem->SetFirstElementType();
		}
	}

	SnakeElements[SnakeElements.Num() - 1]->SetActorLocation(SnakeElements[SnakeElements.Num() - 2]->GetActorLocation());
}

void ASnakeBase::Move() {
	FVector MovementVector(0, 0, 0);
	switch (LastMoveDirection) {
	case EMovementDirection::RIGHT:
		MovementVector.X += ElementSize;
		break;
	case EMovementDirection::LEFT:
		MovementVector.X -= ElementSize;
		break;
	case EMovementDirection::UP:
		MovementVector.Y += ElementSize;
		break;
	case EMovementDirection::DOWN:
		MovementVector.Y -= ElementSize;
		break;
	}

	SnakeElements[0]->ToggleCollision();
	for (int i = SnakeElements.Num() - 1; i > 0; i--) {
		auto CurentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurentElement->SetActorLocation(PrevLocation);
	}
	ASnakeBase::CanChangeHorizontalDirection = true;
	ASnakeBase::CanChangeVerticalDirection = true;

	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other) {
	if (IsValid(OverlappedElement)) {
		int32 ElemIndex;
		SnakeElements.Find(OverlappedElement, ElemIndex);
		bool bIsFirst = ElemIndex == 0;
		IInteractable* InteractableInterface = Cast<IInteractable>(Other);
		if (InteractableInterface) {
			InteractableInterface->Interact(this, bIsFirst);
		}
	}
}

std::vector<FVector> ASnakeBase::GetCoordinate() {
	std::vector<FVector> Coords;
	FVector SnakeCoords;

	for (int i = 0; i < SnakeElements.Num() - 1; i++) {
		SnakeCoords = SnakeElements[i]->GetActorLocation();
		Coords.push_back(SnakeCoords);
	}

	return Coords;
}

FVector ASnakeBase::GetZeroElementCoordinate() {
	return GetCoordinate()[0];
}

void ASnakeBase::UpdateMovementSpeed(float MovementSpeedSnake) {
	SetActorTickInterval(MovementSpeedSnake);
}