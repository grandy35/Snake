// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnBase.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "SnakeBase.h"
#include "Components/InputComponent.h"

bool ASnakeBase::CanChangeVerticalDirection;
bool ASnakeBase::CanChangeHorizontalDirection;

// Sets default values
APlayerPawnBase::APlayerPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
	RootComponent = PawnCamera;
}

// Called when the game starts or when spawned
void APlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(-90, 180, 0));
	CreateSnakeActor();
}

// Called every frame
void APlayerPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Vertical", this, &APlayerPawnBase::HandlePlayerVerticalInput);
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerPawnBase::HandlePlayerHorizontalInput);

}

void APlayerPawnBase::CreateSnakeActor()
{
	SnakeActor = GetWorld()->SpawnActor<ASnakeBase>(SnakeActorClass, FTransform());
}

void APlayerPawnBase::HandlePlayerVerticalInput(float value)
{
	if (IsValid(SnakeActor)) {
		if (value > 0 && SnakeActor->LastMoveDirection != EMovementDirection::DOWN
			&& ASnakeBase::CanChangeVerticalDirection) {
			SnakeActor->LastMoveDirection = EMovementDirection::UP;
			ASnakeBase::CanChangeVerticalDirection = false;
			ASnakeBase::CanChangeHorizontalDirection = false;
		}
		else if (value < 0 && SnakeActor->LastMoveDirection != EMovementDirection::UP
			&& ASnakeBase::CanChangeVerticalDirection) {
			SnakeActor->LastMoveDirection = EMovementDirection::DOWN;
			ASnakeBase::CanChangeVerticalDirection = false;
			ASnakeBase::CanChangeHorizontalDirection = false;
		}
	}
}

void APlayerPawnBase::HandlePlayerHorizontalInput(float value)
{
	if (IsValid(SnakeActor)) {
		if (value > 0 && SnakeActor->LastMoveDirection != EMovementDirection::RIGHT
			&& ASnakeBase::CanChangeHorizontalDirection) {
			SnakeActor->LastMoveDirection = EMovementDirection::LEFT;
			ASnakeBase::CanChangeHorizontalDirection = false;
			ASnakeBase::CanChangeVerticalDirection = false;
		}
		else if (value < 0 && SnakeActor->LastMoveDirection != EMovementDirection::LEFT
			&& ASnakeBase::CanChangeHorizontalDirection) {
			SnakeActor->LastMoveDirection = EMovementDirection::RIGHT;
			ASnakeBase::CanChangeHorizontalDirection = false;
			ASnakeBase::CanChangeVerticalDirection = false;
		}
	}
}
