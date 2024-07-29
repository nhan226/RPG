#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Aura/Interact/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class AURA_API AEnemyCharacter : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	// Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	// end Enemy Interface

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/** end Combat Interface */
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
};
