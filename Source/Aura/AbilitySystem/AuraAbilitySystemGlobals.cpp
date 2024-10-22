#include "AuraAbilitySystemGlobals.h"

#include "Aura/AuraGameplayEffectContext.h"

FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return  new FAuraGameplayEffectContext();
}
