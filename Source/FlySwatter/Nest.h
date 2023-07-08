// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Nest.generated.h"

UCLASS()
class FLYSWATTER_API ANest : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> NestMesh;
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	TObjectPtr<USphereComponent> OverlapCollision;

public:
	// Sets default values for this actor's properties
	ANest();

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
