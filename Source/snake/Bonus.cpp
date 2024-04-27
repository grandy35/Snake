// Fill out your copyright notice in the Description page of Project Settings.


#include "Bonus.h"
#include "SnakeBase.h"
#include "Food.h"
#include "Constants.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "AMyGameState.h"

const float Constants::RecoveryLifeBonus = 0.15f;

// Sets default values
ABonus::ABonus()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

}

// Called when the game starts or when spawned
void ABonus::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABonus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABonus::Interact(AActor* Interactor, bool bIsHead) {
	if (bIsHead) {
		auto Snake = Cast<ASnakeBase>(Interactor);

		if (IsValid(Snake)) {
			Snake->AddSnakeElement();
			Snake->AddSnakeElement();

			AAMyGameState* CurrentGameState = GetWorld()->GetGameState<AAMyGameState>();
			if (CurrentGameState)
			{
				CurrentGameState->RecoveryLifeTime(Constants::RecoveryLifeBonus);
				CurrentGameState->Score += 8;
				CurrentGameState->UpdateScore(CurrentGameState->Score);
			}

			Destroy();
		}
	}


}