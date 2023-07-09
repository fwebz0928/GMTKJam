// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Food.generated.h"

UCLASS()
class FLYSWATTER_API AFood : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Components, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> FoodMesh;
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	TObjectPtr<USphereComponent> PickupCollision;

public:
	// Sets default values for this actor's properties
	AFood();

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMeshComponent* GetFoodMesh() const { return FoodMesh; }

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle DestroyTimer;
};
