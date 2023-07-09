// Copyright Epic Games, Inc. All Rights Reserved.

#include "FlySwatterCharacter.h"

#include "FlyLogging.h"
#include "Food.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"

AFlySwatterCharacter::AFlySwatterCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	TimeRemaining = 60.0f;
	MaxFood = 1;
	MaxHealth = 3;
	CurrentHealth = MaxHealth;
}

void AFlySwatterCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(CountdownTimer, this, &AFlySwatterCharacter::CountdownFinished, 1.0f, true);
	OnTimeUpdated.Broadcast(TimeRemaining);
	OnHealthUpdated.Broadcast(CurrentHealth);
}


void AFlySwatterCharacter::PickupFood(AFood* Food)
{
	if (CurrentCarriedFood > MaxFood || Food == nullptr) return;
	Food->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "FoodSocket");
	CarriedFood.Add(Food);
	CurrentCarriedFood++;
}
void AFlySwatterCharacter::DepositFood()
{
	if (CurrentCarriedFood <= 0) return;

	//Update the score
	Score += CurrentCarriedFood;
	OnScoreUpdated.Broadcast(Score);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ScoreSound, GetActorLocation(), FRotator::ZeroRotator);

	if (Score % 5 == 0)
		IncreaseTime(5.0f);

	//Reset
	CurrentCarriedFood = 0;

	//Get rid of all the food actors attached to the player
	for (auto Food : CarriedFood)
		Food->Destroy();

	CarriedFood.Empty();
}

void AFlySwatterCharacter::DamageHealth()
{
	CurrentHealth = FMath::Clamp(CurrentHealth - 1, 0, MaxHealth);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageSound, GetActorLocation(), FRotator::ZeroRotator);

	OnHealthUpdated.Broadcast(CurrentHealth);
	if (CurrentHealth <= 0)
		DoDeath();

	bCanTakeDamage = false;
	GetWorldTimerManager().SetTimer(IFrameTimer, this, &AFlySwatterCharacter::DisableIFrame, 1.0f, false);
}

void AFlySwatterCharacter::DisableIFrame()
{
	bCanTakeDamage = true;
}
void AFlySwatterCharacter::CountdownFinished()
{
	TimeRemaining = FMath::Clamp(TimeRemaining - 1.0f, 0.0f, 10000.0f);
	OnTimeUpdated.Broadcast(TimeRemaining);
	if (TimeRemaining <= 0.0f)
	{
		//Time Finished
	}
}
void AFlySwatterCharacter::IncreaseTime(const float TimeIncrease)
{
	TimeRemaining += TimeIncrease;
	OnTimeUpdated.Broadcast(TimeRemaining);
}

bool AFlySwatterCharacter::CanCarryMoreFood() const
{
	return CurrentCarriedFood < MaxFood;
}
