// Fill out your copyright notice in the Description page of Project Settings.


#include "AMyGameState.h"
#include "snakeGameModeBase.h"

void AsnakeGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    AAMyGameState* CurrentGameState = GetWorld()->GetGameState<AAMyGameState>();
    if (CurrentGameState)
    {
        CurrentGameState->Score = 0;
    }
}