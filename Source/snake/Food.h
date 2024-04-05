// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractable.h"
#include "Food.generated.h"

UCLASS()
class SNAKE_API AFood : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor, bool bIsHead) override;

	static int food_count;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
		UStaticMeshComponent* MeshComponent;

	UFUNCTION(BlueprintNativeEvent)
		void SetSixFood();
	void SetSixFood_Implementation();
};

bool IsItAvailableCoordinates(FVector coord, float x, float y);

int get_random_number(float size);
