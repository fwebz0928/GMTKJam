// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "FlyLogging.h"
#include "FlySwatterCharacter.h"

// Sets default values
AFood::AFood()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodMesh"));
	RootComponent = FoodMesh;

	PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	PickupCollision->SetupAttachment(RootComponent);
	PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &AFood::OnOverlapBegin);
	InitialLifeSpan = 5;
}

void AFood::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFlySwatterCharacter* PCharacter = Cast<AFlySwatterCharacter>(OtherActor);
	if (PCharacter == nullptr || PCharacter->CanCarryMoreFood() == false) return;

	this->DisableComponentsSimulatePhysics();
	this->SetActorEnableCollision(ECollisionEnabled::NoCollision);
	PCharacter->PickupFood(this);
}
