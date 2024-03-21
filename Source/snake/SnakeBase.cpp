// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include <vector>
#include "IInteractable.h"

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

	Move();
}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; i++) {
		FVector NewLocation;
		if (ElementsNum == 1) {
			NewLocation.X = SnakeElements.Num() * ElementSize + 160;
			NewLocation.Y = 0;
			NewLocation.Z = 0;
		}
		else {
			NewLocation.X = SnakeElements.Num() * ElementSize;
			NewLocation.Y = 0;
			NewLocation.Z = 0;
		}
		FTransform NewTrasform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTrasform);
		NewSnakeElem->SnakeOwner = this;
		int32 ElemIndex = SnakeElements.Add(NewSnakeElem);

		FVector test = SnakeElements[i]->GetTargetLocation();
		UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), test.Y);
		UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), test.X);
		if (ElemIndex == 0) {
			NewSnakeElem->SetFirstElementType();
		}
	}

	//SnakeElements[SnakeElements.Num() - 1]->SetActorLocation(SnakeElements[SnakeElements.Num() - 2]->GetActorLocation());
}

void ASnakeBase::Move()
{
	FVector MovementVector(0, 0, 0);
	MovementSpeed = ElementSize;
	switch (LastMoveDirection) {
	case EMovementDirection::UP:
		MovementVector.X -= MovementSpeed;;
		break;
	case EMovementDirection::DOWN:
		MovementVector.X += MovementSpeed;;
		break;
	case EMovementDirection::LEFT:
		MovementVector.Y -= MovementSpeed;;
		break;
	case EMovementDirection::RIGHT:
		MovementVector.Y += MovementSpeed;;
		break;
	}

	//AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();

	for (int i = SnakeElements.Num() - 1; i > 0; i--) {
		auto CurentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurentElement->SetActorLocation(PrevLocation);
	}
	ASnakeBase::CanChangeHorizontalDirection = true;
	ASnakeBase::CanChangeVerticalDirection = true;
	//UE_LOG(LogTemp, Warning, TEXT("X is: %f"), SnakeElements[0]->GetWorld().Y);
	//UE_LOG(LogTemp, Warning, TEXT("X is: %f"), SnakeElements[0]->GetActorLocation().X);
	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();

	//UE_LOG(LogTemp, Warning, TEXT("Y is: %s"), SnakeElements[0]->GetActorLocation().Y);
	//UE_LOG(LogTemp, Warning, TEXT("X is: %s"), SnakeElements[0]->GetActorLocation().X);
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