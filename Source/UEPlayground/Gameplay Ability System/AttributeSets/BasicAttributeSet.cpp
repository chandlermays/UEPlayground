#include "BasicAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

/*----------------------------------------------------------------
| --- Constructor: Sets default values for BasicAttributeSet --- |
----------------------------------------------------------------*/
UBasicAttributeSet::UBasicAttributeSet()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
	Stamina = 100.0f;
	MaxStamina = 100.0f;
}

/*-----------------------------------------------------------------------
| --- OnRep_Health: Called on clients when the Health is replicated --- |
-----------------------------------------------------------------------*/
void UBasicAttributeSet::OnRep_Health(const FGameplayAttributeData& oldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Health, oldHealth);
}

/*-----------------------------------------------------------------------------
| --- OnRep_MaxHealth: Called on clients when the MaxHealth is replicated --- |
-----------------------------------------------------------------------------*/
void UBasicAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& oldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, MaxHealth, oldMaxHealth);
}

/*-------------------------------------------------------------------------
| --- OnRep_Stamina: Called on clients when the Stamina is replicated --- |
-------------------------------------------------------------------------*/
void UBasicAttributeSet::OnRep_Stamina(const FGameplayAttributeData& oldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Stamina, oldStamina);
}

/*-------------------------------------------------------------------------------
| --- OnRep_MaxStamina: Called on clients when the MaxStamina is replicated --- |
-------------------------------------------------------------------------------*/
void UBasicAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& oldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, MaxStamina, oldMaxStamina);
}

/*-----------------------------------------------------------------------------------------------------
| --- GetLifetimeReplicatedProps: 
-----------------------------------------------------------------------------------------------------*/
void UBasicAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

/*----------------------------------------------------------------------------------------------------
| --- PreAttributeChange: 
----------------------------------------------------------------------------------------------------*/
void UBasicAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

/*----------------------------------------------------------------------------------------------------
| --- PostGameplayExecute:
----------------------------------------------------------------------------------------------------*/
void UBasicAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	// Causes 'PreAttributeChange' to be invoked to update the current values
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(GetHealth());

		if (Data.EffectSpec.Def->GetAssetTags().HasTag(FGameplayTag::RequestGameplayTag("Effects.HitReaction")))
		{
			FGameplayTagContainer HitReactionTagContainer;
			HitReactionTagContainer.AddTag(FGameplayTag::RequestGameplayTag("GameplayAbility.HitReaction"));
			GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(HitReactionTagContainer);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(GetStamina());
	}
}

/*----------------------------------------------------------------------------------------------------
| --- PostGameplayExecute:
----------------------------------------------------------------------------------------------------*/
void UBasicAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetHealthAttribute() && NewValue <= 0.f)
	{
		FGameplayTagContainer DeathAbilityTagContainer;
		DeathAbilityTagContainer.AddTag(FGameplayTag::RequestGameplayTag("GameplayAbility.Death"));
		GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(DeathAbilityTagContainer);
	}
}