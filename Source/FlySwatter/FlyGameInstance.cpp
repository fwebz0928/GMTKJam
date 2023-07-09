// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyGameInstance.h"


void UFlyGameInstance::UpdateHighScore(const int NewScore)
{
	BestScore = NewScore > BestScore ? NewScore : BestScore;
}
