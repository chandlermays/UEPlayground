#include "PlaygroundAbilitySystemComponent.h"

#include "Characters/BaseCharacter.h"

/*-------------------------------------------------------------------------------
| --- Constructor: Sets default values for PlaygroundAbilitySystemComponent --- |
-------------------------------------------------------------------------------*/
UPlaygroundAbilitySystemComponent::UPlaygroundAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

/*----------------------------------------------------------------
| --- BeginPlay: Called when the game starts or when spawned --- |
----------------------------------------------------------------*/
void UPlaygroundAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

/*-------------------------------------------------------------------------------------------------------
| --- OnRep_ActivateAbilities: Called on clients when the activatable abilities array is replicated --- |
-------------------------------------------------------------------------------------------------------*/
void UPlaygroundAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	ABaseCharacter* pCharacter = Cast<ABaseCharacter>(this->GetOwner());
	if (!pCharacter)
		return;
	
	bool bAbilitiesChanged = false;
	
	if (LastActivatableAbilities.Num() != ActivatableAbilities.Items.Num())
	{
		bAbilitiesChanged = true;
	}
	else
	{
		for (int32 i = 0; i < ActivatableAbilities.Items.Num(); ++i)
		{
			if (LastActivatableAbilities[i].Ability != ActivatableAbilities.Items[i].Ability)
			{
				bAbilitiesChanged = true;
				break;
			}
		}
	}
	
	if (bAbilitiesChanged)
	{
		pCharacter->SendAbilitiesChangedEvent();
		LastActivatableAbilities = ActivatableAbilities.Items;
	}
}

/*-------------------------------------------
| --- TickComponent: Called every frame --- |
-------------------------------------------*/
void UPlaygroundAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}