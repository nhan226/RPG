#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Aura/AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraGameModeBase.generated.h"

UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
