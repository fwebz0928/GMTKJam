// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodSpawner.h"

#include "FlyLogging.h"

// Sets default values
AFoodSpawner::AFoodSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	FoodSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FoodSpawnBox"));
	RootComponent = FoodSpawnBox;
}
void AFoodSpawner::BeginPlay()
{
	Super::BeginPlay();

	auto BoxExtent = FoodSpawnBox->GetScaledBoxExtent();
	auto BoxLoc = FoodSpawnBox->GetComponentLocation();
	SpawnBox = FBox(BoxLoc - BoxExtent, BoxLoc + BoxExtent);

	RandomStream.Initialize(FMath::Rand());

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AFoodSpawner::SpawnFood, GetRandomSpawnTimer(), true, 0.0f);
}

float AFoodSpawner::GetRandomSpawnTimer()
{
	return FMath::RandRange(1.5f, 2.5f);
}


void AFoodSpawner::SpawnFood()
{
	FVector RandomPoint = RandomStream.RandPointInBox(SpawnBox);
	FVector EndLoc = RandomPoint - (GetActorUpVector() * 10000.0f);

	DrawDebugLine(GetWorld(), RandomPoint, EndLoc, FColor::Orange, true, 5.0f, 0.0f, 4.0f);

	if (FoodSpawn == nullptr) return;
	auto SpawnedFood = GetWorld()->SpawnActor<AActor>(FoodSpawn, RandomPoint, FRotator::ZeroRotator);
	PRINT_MESSAGE(FColor::Orange, "Spawning Food");
}
