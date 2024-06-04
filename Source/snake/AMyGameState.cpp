// Fill out your copyright notice in the Description page of Project Settings.


#include "AMyGameState.h"
#include "snakeGameModeBase.h"
#include "SnakeBase.h"
#include "BarrierHelper.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

using namespace std;

AAMyGameState::AAMyGameState() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	srand(time(0));
}

void AsnakeGameModeBase::BeginPlay() {
	Super::BeginPlay();

	AAMyGameState* CurrentGameState = GetWorld()->GetGameState<AAMyGameState>();
	if (CurrentGameState) {
		CurrentGameState->Score = 0;
		CurrentGameState->LifeTime = 1.0;
		CurrentGameState->CurrentLifeTime = CurrentGameState->LifeTime;

		CurrentGameState->Time = 0;
	}
}

void AAMyGameState::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	Time += DeltaTime;
	if (Time >= 0.5f) {
		DecreaseLifeTime();
		UpdateLifeTime(CurrentLifeTime);
		Time -= 0.5f;
	}

	if (CurrentLifeTime <= 0) {
		MySnake->Destroy();
		if (!MySnake) {
			AAMyGameState::LoseScreen();
		}
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AAMyGameState::DecreaseLifeTime() {
	CurrentLifeTime -= DecreaseLife;
	if (CurrentLifeTime <= 0) {
		CurrentLifeTime = 0;
		AAMyGameState::LoseScreen();
	}
}

void AAMyGameState::RecoveryLifeTime(float RecoveryLife) {
	CurrentLifeTime += RecoveryLife;
	if (CurrentLifeTime > LifeTime) {
		CurrentLifeTime = LifeTime;
	}
}

void AAMyGameState::IncreaseScore(int PlusScore, EMovementDirection LastMoveDirection, FVector SnakeHead) {
	Score += PlusScore;
	UpdateScore(Score);

	if (PlusScore == 4 && BarrierHelper != NULL) {
		EatedFood++;
		if (EatedFood == 4) {
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABarrier::StaticClass(), FoundActors);

			for (AActor* Actor : FoundActors) {
				ABarrier* BarrierActor = Cast<ABarrier>(Actor);
				if (BarrierActor) {
					BarrierActor->Destroy();
				}
			}

			EatedFood = 0;
			BarrierHelper = NULL;
		}
	}

	if ((Score % 32) == 0 && BarrierHelper == NULL) {
		BarrierHelper = NewObject<UBarrierHelper>();
		vector<BarrierPosition> Positions = BarrierHelper->HandleBarrier(GetWorld(), LastMoveDirection, SnakeHead);
		UWorld* CurrentWorld = GetWorld();

		if (Positions.empty()) {
			return;
		}
		
		IsImaginaryObject = BarrierHelper->IsImaginaryBarrier();

		for (int i = 0; i < Positions.size(); i++) {
			auto CurrentBarrier = CurrentWorld->SpawnActor<ABarrier>(Barrier, Positions[i].Location, Positions[i].Rotation);
			if (Positions[i].Scale.IsSet()) {
				CurrentBarrier->SetActorScale3D(Positions[i].Scale.GetValue());
			}
		}
	}

	if (Score == 500) {
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		AAMyGameState::WinScreen();
	}
}