// Fill out your copyright notice in the Description page of Project Settings.


#include "Bonus.h"
#include "SnakeBase.h"
#include "Food.h"
#include "Constants.h"

// Sets default values
ABonus::ABonus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

bool HasCollisions(std::vector<FVector> coords, float x, float y) {
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

void ABonus::Interact(AActor* Interactor, bool bIsHead) {
	if (bIsHead) {
		auto Snake = Cast<ASnakeBase>(Interactor);

		if (IsValid(Snake)) {
			auto coords = Snake->GetCoordinate();
			bool hasCollision = true;
			float x = 0;
			float y = 0;
			while (hasCollision) {
				x = get_random_number(Snake->ElementSize);
				y = get_random_number(Snake->ElementSize);

				if (!HasCollisions(coords, x, y)) {
					hasCollision = false;
				}
			}

			FRotator BonusRotation(0, 0, 0);
			FVector BonusLocation(x, y, 0);

			if (AFood::food_count == threshold_food_eating) {
				auto SpawnedBonus = GetWorld()->SpawnActor<ABonus>(this->GetClass(), BonusLocation, BonusRotation);

				AFood::food_count = 0;
			}
			Destroy();
		}
	}
}

