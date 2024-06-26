// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrier.h"
#include "AMyGameState.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
ABarrier::ABarrier()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent")); // remove this
}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABarrier::Interact(AActor* Interactor, bool bIsHead) {
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