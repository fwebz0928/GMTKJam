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
}
void AFood::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AFood::DestroyFood, 5.0f, false, 0.0f);
}
void AFood::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFlySwatterCharacter* PCharacter = Cast<AFlySwatterCharacter>(OtherActor);
	if (PCharacter == nullptr) return;

	this->DisableComponentsSimulatePhysics();
	this->SetActorEnableCollision(ECollisionEnabled::NoCollision);
	PCharacter->PickupFood(this);
}


void AFood::DestroyFood()
{
	this->Destroy();
}
