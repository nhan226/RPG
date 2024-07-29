#include "EnemyCharacter.h"

#include "Aura/Aura.h"

#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	abilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	abilitySystemComponent->SetIsReplicated(true);
	abilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	attributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	abilitySystemComponent->InitAbilityActorInfo(this, this);
	// NOTED: Bind delegate function when gameplay effect apllied.
	Cast<UAuraAbilitySystemComponent>(abilitySystemComponent)->AbilityActorInfoSet();
}

void AEnemyCharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	weapon->SetRenderCustomDepth(true);
}

void AEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	weapon->SetRenderCustomDepth(false);
}

int32 AEnemyCharacter::GetPlayerLevel()
{
	return Level;
}