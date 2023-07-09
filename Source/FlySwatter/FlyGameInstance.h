// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FlyGameInstance.generated.h"


UCLASS()
class FLYSWATTER_API UFlyGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Score")
	int BestScore;

	UFUNCTION(BlueprintCallable)
	void UpdateHighScore(int NewScore);
};
