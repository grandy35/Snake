// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SnakeBase.h"
#include "Buff.generated.h"

UENUM()
enum BuffType
{
	Haste,
	SlowDown,
	Afk,

	BuffTypeMax,
};

UCLASS()
class SNAKE_API UBuff : public UObject
{
	GENERATED_BODY()


public:
	void HandleBuff(AActor* ActorToApply, BuffType Buff, float LifeDuration);
	void RemoveBuff(ASnakeBase* Snake, BuffType Buff);

private:
	BuffType Type;
	float TimeDuration;
	float LastMovementSpeed;

	void SpeedUp(ASnakeBase* Snake);
	void SlowingDown(ASnakeBase* Snake);
	void Afk(ASnakeBase* Snake);
	void UnAfk(ASnakeBase* Snake);

	float GetTimerDuration();
};
