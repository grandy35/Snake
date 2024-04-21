// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "SnakeBase.h"
#include "Bonus.h"
#include "Constants.h"
#include "cstdlib"
#include "cmath"
#include "ctime"
#include <string>
#include <vector>
#include "AMyGameState.h"

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

bool HasCollision(std::vector<FVector> coords, float x, float y) {
	bool isCollision = false;
	for (int i = 0; i < coords.size(); i++) {
		bool isAvailable = IsItAvailableCoordinates(coords[i], x, y);
		if (!isAvailable) {
			isCollision = true;
			break;
		}
	}

	return isCollision;
}

int get_random_number(float size) {
	int field_length = 984;
	int half_field_length = field_length / 2;

	std::vector<int> multiples;

	for (int i = -half_field_length; i <= half_field_length; i++)
	{
		if (i % 41 == 0)
		{
			multiples.push_back(i);
		}
	}

	int index = rand() % multiples.size();

	return multiples[index];
}

bool IsItAvailableCoordinates(FVector coord, float x, float y) {

	if (coord.X != x && coord.Y != y) {
		return true;
	}
	return false;
}

void AFood::Interact(AActor* Interactor, bool bIsHead) {
	if (bIsHead) {
		auto Snake = Cast<ASnakeBase>(Interactor);

		if (IsValid(Snake)) {
			Snake->AddSnakeElement();

			auto coords = Snake->GetCoordinate();
			bool hasCollision = true;

			float XFood = 0;
			float YFood = 0;
			float XBonus = 0;
			float YBonus = 0;

			while (hasCollision) {
				XFood = get_random_number(Snake->ElementSize);
				YFood = get_random_number(Snake->ElementSize);

				if (!HasCollision(coords, XFood, YFood)) {
					hasCollision = false;
				}
				XBonus = get_random_number(Snake->ElementSize);
				YBonus = get_random_number(Snake->ElementSize);

				if (!HasCollision(coords, XBonus, YBonus)) {
					hasCollision = false;
				}
				if (XFood != XBonus && YFood != YBonus) {
					hasCollision = false;
				}
			}


			AAMyGameState* CurrentGameState = GetWorld()->GetGameState<AAMyGameState>();
			if (CurrentGameState)
			{
				CurrentGameState->Score += 4;
				UE_LOG(LogTemp, Warning, TEXT("Updating score to %d"), CurrentGameState->Score);
				CurrentGameState->UpdateScore(CurrentGameState->Score);
			}

			FRotator FoodRotation(0, 0, 0);
			FVector FoodLocation(XFood, YFood, 0);
			FRotator BonusRotation(0, 0, 0);
			FVector BonusLocation(XBonus, YBonus, 0);

			auto SpawnedFood = GetWorld()->SpawnActor<AFood>(this->GetClass(), FoodLocation, FoodRotation);

			int food_count = Snake->SnakeElements.Num();

			if ((food_count % threshold_food_eating) == 0) {
				auto SpawnedBonus = GetWorld()->SpawnActor<ABonus>(Bonus, BonusLocation, BonusRotation);
			}

			Destroy();
		}
	}
}