#include "PlayerCharacter.h"

#include "AbilitySystemComponent.h"

#include "Aura/UI/HUD/AuraHUD.h"
#include "Aura/PlayerState/AuraPlayerState.h"
#include "Aura/PlayerController/AuraPlayerController.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

int32 APlayerCharacter::GetPlayerLevel()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void APlayerCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* auraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);
	auraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(auraPlayerState, this);
	// NOTED: Bind delegate function when gameplay effect apllied.
	Cast<UAuraAbilitySystemComponent>(auraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	abilitySystemComponent = auraPlayerState->GetAbilitySystemComponent();
	attributeSet = auraPlayerState->GetAttributeSet();
	
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			// Init and setup widget controller params.
			AuraHUD->InitOverlay(AuraPlayerController, auraPlayerState, abilitySystemComponent, attributeSet);
		}
	}

	// Init Primary attributes
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.0f);
	// Init Secondary attributes
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.0f);
	// Init Vital attributes
	ApplyEffectToSelf(DefaultVitalAttributes, 1.0f);
}
