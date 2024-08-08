#include "EnemyCharacter.h"

#include "Aura/Aura.h"

#include "Aura/UI/Widgets/AuraUserWidget.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"

#include "Components/WidgetComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	abilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	abilitySystemComponent->SetIsReplicated(true);
	abilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	attributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	// Bind delegate when attribute changed new value.
	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(attributeSet))
	{
		abilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		abilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		// Initial Value for health and maxhealth
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	abilitySystemComponent->InitAbilityActorInfo(this, this);
	// NOTED: Bind delegate function when gameplay effect apllied.
	Cast<UAuraAbilitySystemComponent>(abilitySystemComponent)->AbilityActorInfoSet();

	// Init Primary attributes
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.0f);
	// Init Secondary attributes
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.0f);
	// Init Vital attributes
	ApplyEffectToSelf(DefaultVitalAttributes, 1.0f);
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