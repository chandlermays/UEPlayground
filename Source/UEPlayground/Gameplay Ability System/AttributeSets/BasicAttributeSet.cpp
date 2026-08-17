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
	Damage = 0.0f;
	Shield = 0.0f;
	MaxShield = 100.0f;
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

/*-----------------------------------------------------------------------
| --- OnRep_Shield: Called on clients when the Shield is replicated --- |
-----------------------------------------------------------------------*/
void UBasicAttributeSet::OnRep_Shield(const FGameplayAttributeData& oldShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Shield, oldShield);
}

/*-----------------------------------------------------------------------------
| --- OnRep_MaxShield: Called on clients when the MaxShield is replicated --- |
-----------------------------------------------------------------------------*/
void UBasicAttributeSet::OnRep_MaxShield(const FGameplayAttributeData& oldMaxShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, MaxShield, oldMaxShield);
}

/*----------------------------------------------------------------------------------------------------------------------
| --- GetLifetimeReplicatedProps: Defines the properties that should be replicated over the network for this class --- |
----------------------------------------------------------------------------------------------------------------------*/
void UBasicAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

/*------------------------------------------------------------------------------------------------------------------------------------------
| --- PreAttributeChange: Called just before any modification happens to an attribute's base value when an attribute aggregator exists --- |
------------------------------------------------------------------------------------------------------------------------------------------*/
void UBasicAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());
	}
}

/*----------------------------------------------------------------------------------------------------------------------
| --- PostGameplayExecute: Called just after a GameplayEffect is executed to modify the base value of an attribute --- |
----------------------------------------------------------------------------------------------------------------------*/
void UBasicAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float TotalDamage = GetDamage();
		SetDamage(0.0f);
		
		float CurrentShield = GetShield();
		if (CurrentShield > 0.0f)
		{
			SetShield(CurrentShield - TotalDamage);
			float RemainingDamage = TotalDamage - CurrentShield;
			
			if (RemainingDamage > 0.0f)
			{
				SetHealth(GetHealth() - RemainingDamage);
			}
		}
		else
		{
			SetHealth(GetHealth() - TotalDamage);
		}
		
		if (Data.EffectSpec.Def->GetAssetTags().HasTag(FGameplayTag::RequestGameplayTag("Effects.HitReaction")) && Data.EvaluatedData.Magnitude != 0.f)
		{
			FGameplayTagContainer HitReactionTagContainer;
			HitReactionTagContainer.AddTag(FGameplayTag::RequestGameplayTag("GameplayAbility.HitReaction"));
			GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(HitReactionTagContainer);
		}
	}
	
	// Causes 'PreAttributeChange' to be invoked to update the current values
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(GetHealth());
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(GetStamina());
	}
}

/*----------------------------------------------------------------------------------------------------------------------
| --- PostGameplayExecute: Called just after a GameplayEffect is executed to modify the base value of an attribute --- |
----------------------------------------------------------------------------------------------------------------------*/
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