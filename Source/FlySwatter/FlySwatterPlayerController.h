// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "FlySwatterPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateDashCharge, int, CurrentDash);

UCLASS()
class AFlySwatterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFlySwatterPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* DashInputAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationTouchAction;

	UPROPERTY(BlueprintAssignable)
	FUpdateDashCharge OnDashChargeUpdated;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void Dash();
	void OnSetDestinationReleased();
	void RechargeDashCharges();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetChargePercentage();
	void OnTouchTriggered();
	void OnTouchReleased();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDash(FVector DashLoc);


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dash")
	int MaxDashCharges;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dash")
	int CurrentDashCharges;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dash")
	float DashCooldown;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dash")
	FTimerHandle DashRechargeHandle;

private:
	FVector CachedDestination;

	bool bCanDash;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};
