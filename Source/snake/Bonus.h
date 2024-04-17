// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractable.h"
#include "Bonus.generated.h"

UCLASS()
class SNAKE_API ABonus : public AActor, public IInteractable
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	ABonus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
		UStaticMeshComponent* MeshComponent;

	UFUNCTION(BlueprintImplementableEvent, Category = "Score Update")
		void UpdateScore(int NewScore);

	virtual void Interact(AActor* Interactor, bool bIsHead) override;

};