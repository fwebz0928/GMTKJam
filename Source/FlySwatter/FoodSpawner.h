// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "FoodSpawner.generated.h"

UCLASS()
class FLYSWATTER_API AFoodSpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	TObjectPtr<UBoxComponent> FoodSpawnBox;

public:
	// Sets default values for this actor's properties
	AFoodSpawner();

protected:
	virtual void BeginPlay() override;
	float GetRandomPoisonTimer();
	float GetRandomSpawnTimer();
	void SpawnPoison();

	UPROPERTY()
	FBox SpawnBox;
	UPROPERTY()
	FTimerHandle SpawnTimerHandle;
	UPROPERTY()
	FTimerHandle PoisonSpawnTimer;
	UPROPERTY()
	FRandomStream RandomStream;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Food")
	TSubclassOf<AActor> FoodSpawn;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Food")
	TSubclassOf<AActor> PoisonSpawn;
	void SpawnFood();
};
