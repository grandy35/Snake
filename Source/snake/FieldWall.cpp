// Fill out your copyright notice in the Description page of Project Settings.

#include "AMyGameState.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "FieldWall.h"

// Sets default values
AFieldWall::AFieldWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFieldWall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFieldWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFieldWall::Interact(AActor* Interactor, bool bIsHead) {
	if (bIsHead) {
		auto Snake = Cast<ASnakeBase>(Interactor);
		AAMyGameState* CurrentGameState = GetWorld()->GetGameState<AAMyGameState>();
		if (CurrentGameState && !CurrentGameState->IsImaginaryObject) {
			GetWorld()->DestroyActor(Snake);

			UGameplayStatics::SetGamePaused(GetWorld(), true);
			CurrentGameState->LoseScreen();
		}
	}
}
