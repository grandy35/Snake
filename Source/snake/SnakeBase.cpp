// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include <vector>
// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 10.f;
	LastMoveDirection = EMovementDirection::UP;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(4);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; i++) {
		FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTrasform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTrasform);
		int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
		if (ElemIndex == 0) {
			NewSnakeElem->SetFirstElementType();
		}
	}
}

void ASnakeBase::Move(float DeltaTime)
{
	FVector MovementVector(0,0,0);
	MovementSpeed = ElementSize;
	
	switch (LastMoveDirection) {
	case EMovementDirection::UP:
		MovementVector.X -= MovementSpeed;
		break;
	case EMovementDirection::DOWN:
		MovementVector.X += MovementSpeed;
		break;
	case EMovementDirection::LEFT:
		MovementVector.Y -= MovementSpeed;
		break;
	case EMovementDirection::RIGHT:
		MovementVector.Y += MovementSpeed;
		break;
	}
	//AddActorWorldOffset(MovementVector);

	for (int i = SnakeElements.Num() - 1; i > 0; i--) {
		auto CurentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurentElement->SetActorLocation(PrevLocation);
	}
	ASnakeBase::CanChangeHorizontalDirection = true;
	ASnakeBase::CanChangeVerticalDirection = true;
	SnakeElements[0]->AddActorWorldOffset(MovementVector);
}

