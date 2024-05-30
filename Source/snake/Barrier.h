// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IInteractable.h"
#include "SnakeBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barrier.generated.h"

UCLASS()
class SNAKE_API ABarrier : public AActor, public IInteractable {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABarrier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor, bool bIsHead) override;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
		UStaticMeshComponent* MeshComponent;
};
