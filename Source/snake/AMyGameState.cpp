// Fill out your copyright notice in the Description page of Project Settings.


#include "AMyGameState.h"
#include "snakeGameModeBase.h"
#include "SnakeBase.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

AAMyGameState::AAMyGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AsnakeGameModeBase::BeginPlay() {
	Super::BeginPlay();

	AAMyGameState* CurrentGameState = GetWorld()->GetGameState<AAMyGameState>();
	if (CurrentGameState)
	{
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

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}

}


void AAMyGameState::DecreaseLifeTime() {
	CurrentLifeTime -= DecreaseLife;
	if (CurrentLifeTime <= 0) {
		CurrentLifeTime = 0;

	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("LifeTimeDecreased %f"), CurrentLifeTime));
}

void AAMyGameState::RecoveryLifeTime(float RecoveryLife) {
	CurrentLifeTime += RecoveryLife;
	if (CurrentLifeTime > LifeTime) {
		CurrentLifeTime = LifeTime;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LifeTimeIncreased %f"), CurrentLifeTime));
}