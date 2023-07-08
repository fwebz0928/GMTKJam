// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FlySwatterCharacter.generated.h"

class AFood;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreUpdate, int, NewScore);


UCLASS(Blueprintable)
class AFlySwatterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFlySwatterCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	void PickupFood(AFood* Food);
	void DepositFood();

	UPROPERTY(BlueprintAssignable)
	FScoreUpdate OnScoreUpdated;

protected:
	UPROPERTY()
	int MaxFood;
	UPROPERTY()
	int CurrentCarriedFood;

	UPROPERTY()
	TArray<TObjectPtr<AFood>> CarriedFood;

	UPROPERTY()
	int Score;
};
