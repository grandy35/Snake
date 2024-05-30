// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "Constants.h"
#include "SnakeBase.h"
#include "Constants.h"
#include "cstdlib"
#include "cmath"
#include "ctime"
#include <string>
#include <vector>
#include "AMyGameState.h"

const int Constants::ThresholdFoodEating = 6;

const float Constants::RecoveryLifeFood = 0.1f;

// Sets default values
AFood::AFood()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	srand(time(0));
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int GetRandomNumber(int Size) {
	int HalfFieldLength = Constants::FieldLength / 2;

	std::vector<int> Multiples;

	for (int i = -HalfFieldLength; i <= HalfFieldLength; i++)
	{
		if (i % Size == 0)
		{
			Multiples.push_back(i);
		}
	}

	int Index = rand() % Multiples.size();

	return Multiples[Index];
}



void AFood::Interact(AActor* Interactor, bool bIsHead) {
	if (bIsHead) {
		auto Snake = Cast<ASnakeBase>(Interactor);

		if (IsValid(Snake)) {
			Snake->AddSnakeElement();

			AAMyGameState* CurrentGameState = GetWorld()->GetGameState<AAMyGameState>();
			if (CurrentGameState) {
				CurrentGameState->RecoveryLifeTime(Constants::RecoveryLifeFood);
				FVector SnakeHead = Snake->GetZeroElementCoordinate();
				CurrentGameState->IncreaseScore(4, Snake->LastMoveDirection, SnakeHead);
			}

			int Tries = 0;
			float SphereSize = float(25.0f);
			FRotator Rotation(0, 0, 0);

			while (Tries < Constants::MaxRegenerationCount) {
				float XFood = GetRandomNumber(Snake->ElementSize);
				float YFood = GetRandomNumber(Snake->ElementSize);

				FVector FoodLocation(XFood, YFood, 0);
				if (!IsLocationFree(FoodLocation, SphereSize)) {
					Tries++;
					continue;
				}
				auto SpawnedFood = GetWorld()->SpawnActor<AFood>(this->GetClass(), FoodLocation, Rotation);

				break;
			}

			int Try = 0;
			int Food_Count = Snake->SnakeElements.Num();

			if ((Food_Count % Constants::ThresholdFoodEating) == 0) {
				while (Try < Constants::MaxRegenerationCount) {
					float XBonus = GetRandomNumber(Snake->ElementSize);
					float YBonus = GetRandomNumber(Snake->ElementSize);
					FVector BonusLocation(XBonus, YBonus, 0);

					if (!IsLocationFree(BonusLocation, SphereSize)) {
						Try++;
						continue;
					}

					auto SpawnedBonus = GetWorld()->SpawnActor<ABonus>(Bonus, BonusLocation, Rotation);
					SpawnedBonus->SelfDestroy(5);

					break;
				}
			}
		}
	}
}

bool AFood::IsLocationFree(FVector Location, float SphereSize) {
	FCollisionShape Circle = FCollisionShape::MakeSphere(SphereSize);
	return !GetWorld()->OverlapAnyTestByChannel(Location, FQuat::Identity, ECollisionChannel::ECC_WorldDynamic, Circle);
}