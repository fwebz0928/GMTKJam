// Fill out your copyright notice in the Description page of Project Settings.


#include "PoisonSplat.h"


// Sets default values
APoisonSplat::APoisonSplat()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PoisonBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PoisonBall"));
	RootComponent = PoisonBall;
	
	PoisonSplat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PoisonSplat"));
	PoisonSplat->SetupAttachment(RootComponent);
	
	OverlapCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	OverlapCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APoisonSplat::BeginPlay()
{
	Super::BeginPlay();
	PoisonSplat->SetWorldScale3D(FVector::Zero());
	//InitializePoison();
}
