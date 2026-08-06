#include "PlaygroundGameplayAbility.h"

/*------------------------------------------------------------------------
| --- Constructor: Sets default values for PlaygroundGameplayAbility --- |
------------------------------------------------------------------------*/
UPlaygroundGameplayAbility::UPlaygroundGameplayAbility()
{
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("GameplayAbility.Active"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Dead"));
}

/*-------------------------------------------------------------------------------------
| --- HasPC: Helper function to check if the ability has a valid PlayerController --- |
-------------------------------------------------------------------------------------*/
bool UPlaygroundGameplayAbility::HasPC() const
{
	const APawn* PawnObject = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!PawnObject)
		return false;

	return PawnObject->GetController()->IsA<APlayerController>();
}