// Copyright Epic Games, Inc. All Rights Reserved.

#include "FlySwatterCharacter.h"

#include "Food.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"

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
}

void AFlySwatterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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

	//Reset
	CurrentCarriedFood = 0;

	//Get rid of all the food actors attached to the player
	for (auto Food : CarriedFood)
		Food->Destroy();

	CarriedFood.Empty();
}
