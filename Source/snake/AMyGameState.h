// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "AMyGameState.generated.h"

/**
 *
 */
UCLASS()
class SNAKE_API AAMyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
		int Score;

	UFUNCTION(BlueprintImplementableEvent, Category = "Score Update")
		void UpdateScore(int NewScore);
};
