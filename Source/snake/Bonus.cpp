// Fill out your copyright notice in the Description page of Project Settings.


#include "Bonus.h"
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
	srand(time(0));
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
			AAMyGameState* CurrentGameState = GetWorld()->GetGameState<AAMyGameState>();
			if (CurrentGameState) {
				BuffType SelectedBuff = static_cast<BuffType>(rand() % BuffTypeMax);
				FVector SnakeHead = Snake->GetZeroElementCoordinate();
				switch (SelectedBuff) { // move to method
					case BuffType::Haste:
						Snake->AddSnakeElement();
						Snake->AddSnakeElement();
						ApplyBuff(Snake, Haste, 5);
						CurrentGameState->IncreaseScore(8, Snake->LastMoveDirection, SnakeHead);
						CurrentGameState->RecoveryLifeTime(Constants::RecoveryLifeBonus);
						break;
					case BuffType::SlowDown:
						ApplyBuff(Snake, SlowDown, 5);
						CurrentGameState->IncreaseScore(-4, Snake->LastMoveDirection, SnakeHead);
						break;
					case BuffType::Afk:
						ApplyBuff(Snake, Afk, 5);
						CurrentGameState->IncreaseScore(-12, Snake->LastMoveDirection, SnakeHead);
						break;
				}
			}

			Destroy();
		}
	}
}

void ABonus::ApplyBuff(ASnakeBase* Snake, BuffType Buff, float TimeDuration) {
    UBuff* BuffToApply = NewObject<UBuff>();
    BuffToApply->HandleBuff(Snake, Buff, TimeDuration);

	FTimerHandle TimerHandler;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandler,
        [BuffToApply, Snake, Buff, this] { BuffToApply->RemoveBuff(Snake, Buff); },
        TimeDuration,
        false);
}

void ABonus::SelfDestroy(float DestroyDelay) {
	FTimerHandle TimerHandler;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandler,
		[this] { this->Destroy(); },
		DestroyDelay,
		false);
}
