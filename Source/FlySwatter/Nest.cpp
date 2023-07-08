// Fill out your copyright notice in the Description page of Project Settings.


#include "Nest.h"

#include "FlySwatterCharacter.h"

// Sets default values
ANest::ANest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	NestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodMesh"));
	RootComponent = NestMesh;

	OverlapCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	OverlapCollision->SetupAttachment(RootComponent);
	OverlapCollision->OnComponentBeginOverlap.AddDynamic(this, &ANest::OnOverlapBegin);
}
void ANest::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto PCharacter{Cast<AFlySwatterCharacter>(OtherActor)};
	if (PCharacter == nullptr) return;

	PCharacter->DepositFood();
}
