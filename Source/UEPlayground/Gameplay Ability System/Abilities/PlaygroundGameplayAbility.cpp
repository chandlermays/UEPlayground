#include "PlaygroundGameplayAbility.h"

UPlaygroundGameplayAbility::UPlaygroundGameplayAbility()
{
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Active")));
}