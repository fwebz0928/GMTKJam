// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FlySwatterCharacter.generated.h"

class AFood;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreUpdate, int, NewScore);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeUpdate, float, Time);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthUpdated, int, Health);


UCLASS(Blueprintable)
class AFlySwatterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFlySwatterCharacter();

	void PickupFood(AFood* Food);
	void DepositFood();
	UFUNCTION(BlueprintCallable)
	void DamageHealth();
	void DisableIFrame();
	void CountdownFinished();
	void IncreaseTime(float TimeIncrease);
	bool CanCarryMoreFood() const;

	UPROPERTY(BlueprintAssignable)
	FScoreUpdate OnScoreUpdated;
	UPROPERTY(BlueprintAssignable)
	FTimeUpdate OnTimeUpdated;
	UPROPERTY(BlueprintAssignable)
	FHealthUpdated OnHealthUpdated;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void DoDeath();

	UPROPERTY()
	int MaxFood;
	UPROPERTY()
	int CurrentCarriedFood;

	UPROPERTY()
	TArray<TObjectPtr<AFood>> CarriedFood;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Score")
	int Score;
	UPROPERTY()
	bool bCanTakeDamage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	USoundBase* ScoreSound;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	USoundBase* DamageSound;

	UPROPERTY()
	int MaxHealth;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Health")
	int CurrentHealth;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Health")
	FTimerHandle CountdownTimer;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Health")
	FTimerHandle IFrameTimer;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Timer")
	float TimeRemaining;
};
