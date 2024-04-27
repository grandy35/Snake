// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SnakeBase.h"
#include "AMyGameState.generated.h"

/**
 *
 */
UCLASS()
class SNAKE_API AAMyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AAMyGameState();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
		int Score;

	UFUNCTION(BlueprintImplementableEvent, Category = "Score Update")
		void UpdateScore(int NewScore);


	UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "LifeTime", meta = (ClampMin = "0", ClampMax = "1"))
		float LifeTime;

	UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "LifeTime")
		float CurrentLifeTime;

	UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "LifeTime")
		float DecreaseLife;

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateLifeTime(float NewCurrentLifeTime);

	void DecreaseLifeTime();

	void RecoveryLifeTime(float RecoveryLife);

	float Time;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snake")
		ASnakeBase* MySnake;

};
