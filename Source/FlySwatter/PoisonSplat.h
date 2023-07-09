// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PoisonSplat.generated.h"

UCLASS()
class FLYSWATTER_API APoisonSplat : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> PoisonSplat;
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> PoisonBall;
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	TObjectPtr<USphereComponent> OverlapCollision;

public:
	APoisonSplat();

protected:
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMeshComponent* GetPoisonBallMesh() const { return PoisonBall; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMeshComponent* GetSplatMesh() const { return PoisonSplat; }


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitializePoison();
};
