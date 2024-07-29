#include "AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Aura/Interact/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(auraContext);

	UEnhancedInputLocalPlayerSubsystem* inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (inputSubsystem) 
	{
		inputSubsystem->AddMappingContext(auraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI inputActionData;
	inputActionData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputActionData.SetHideCursorDuringCapture(false);
	SetInputMode(inputActionData);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	enhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& inputActionValue)
{
	const FVector2D inputAxisVector = inputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// Get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* controlledPawn = GetPawn<APawn>()) 
	{
		controlledPawn->AddMovementInput(ForwardDirection, inputAxisVector.Y);
		controlledPawn->AddMovementInput(RightDirection, inputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult cursorHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, cursorHit);
	if (!cursorHit.bBlockingHit) return;

	lastActor = thisActor;
	thisActor = cursorHit.GetActor();

	/**
	 * Line trace from cursor. There are several scenarios:
	 *  A. LastActor is null && ThisActor is null
	 *		- Do nothing
	 *	B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 *	C. LastActor is valid && ThisActor is null
	 *		- UnHighlight LastActor
	 *	D. Both actors are valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor, and Highlight ThisActor
	 *	E. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */

	if (lastActor == nullptr)
	{
		if (thisActor != nullptr)
		{
			// Case B
			thisActor->HighlightActor();
		}
		else
		{
			// Case A - both are null, do nothing
		}
	}
	else // LastActor is valid
	{
		if (thisActor == nullptr)
		{
			// Case C
			lastActor->UnHighlightActor();
		}
		else // both actors are valid
		{
			if (lastActor != thisActor)
			{
				// Case D
				lastActor->UnHighlightActor();
				thisActor->HighlightActor();
			}
			else
			{
				// Case E - do nothing
			}
		}
	}
}
