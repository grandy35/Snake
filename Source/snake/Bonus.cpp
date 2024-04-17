// Fill out your copyright notice in the Description page of Project Settings.


#include "Bonus.h"
#include "SnakeBase.h"
#include "Food.h"
#include "Constants.h"
#include "EngineUtils.h"

// Sets default values
ABonus::ABonus()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

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
			AFood::InnerScore += 8;
			UpdateScore(AFood::InnerScore);

			Destroy();
		}
	}


}