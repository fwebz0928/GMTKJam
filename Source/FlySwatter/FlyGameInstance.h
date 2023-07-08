// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FlyGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FBestTime
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time")
	int Minutes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time")
	int Seconds;
};

UCLASS()
class FLYSWATTER_API UFlyGameInstance : public UGameInstance
{
	GENERATED_BODY()


	

protected:

	UPROPERTY()
	FBestTime BestTime{0, 0};
};
