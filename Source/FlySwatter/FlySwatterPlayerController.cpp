// Copyright Epic Games, Inc. All Rights Reserved.

#include "FlySwatterPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "FlySwatterCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FlyLogging.h"
#include "Kismet/GameplayStatics.h"

AFlySwatterPlayerController::AFlySwatterPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AFlySwatterPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		Subsystem->AddMappingContext(DefaultMappingContext, 0);

	MaxDashCharges = 1;
	CurrentDashCharges = MaxDashCharges;
	DashCooldown = 1.0f;
}

void AFlySwatterPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AFlySwatterPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AFlySwatterPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AFlySwatterPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AFlySwatterPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AFlySwatterPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AFlySwatterPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AFlySwatterPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AFlySwatterPlayerController::OnTouchReleased);


		EnhancedInputComponent->BindAction(DashInputAction, ETriggerEvent::Started, this, &AFlySwatterPlayerController::Dash);
	}
}

void AFlySwatterPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AFlySwatterPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AFlySwatterPlayerController::Dash()
{
	if (CurrentDashCharges == 0)return;

	PRINT_MESSAGE(FColor::Orange, "Dashing | Dash Amount %d", CurrentDashCharges);

	if (GetWorldTimerManager().IsTimerActive(DashRechargeHandle))
		GetWorldTimerManager().ClearTimer(DashRechargeHandle);

	FVector PawnLocation = GetPawn()->GetActorLocation();
	FVector2D MousePosition;
	FVector MouseLocation, MouseDirection;

	this->GetMousePosition(MousePosition.X, MousePosition.Y);
	this->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);

	FVector PlaneNormal(0.f, 0.f, 1.f);

	FVector EndLocation = FMath::LinePlaneIntersection(MouseLocation, MouseLocation + (MouseDirection * 10000.f), PawnLocation, PlaneNormal);

	EndLocation.Z = PawnLocation.Z;

	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("Trace")), false, this);
	GetWorld()->LineTraceSingleByChannel(HitResult, PawnLocation, EndLocation, ECC_Visibility, TraceParams);

	if (HitResult.IsValidBlockingHit())
		// Draw a debug line to visualize the trace
		DrawDebugLine(GetWorld(), PawnLocation, HitResult.Location, FColor::Red, false, 5.0f, 0, 5.0f);
	else
		// Draw a debug line to visualize the trace from the pawn to the cursor
		DrawDebugLine(GetWorld(), PawnLocation, EndLocation, FColor::Red, false, 5.0f, 0, 5.0f);

	GetWorldTimerManager().SetTimer(DashRechargeHandle, this, &AFlySwatterPlayerController::RechargeDashCharges, DashCooldown, true, 0.0f);
}


void AFlySwatterPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

void AFlySwatterPlayerController::RechargeDashCharges()
{
	CurrentDashCharges = FMath::Clamp(CurrentDashCharges + 1, 0, MaxDashCharges);
	if (CurrentDashCharges == MaxDashCharges)
		GetWorldTimerManager().ClearTimer(DashRechargeHandle);
}


// Triggered every frame when the input is held down
void AFlySwatterPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AFlySwatterPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}
