#include "BasicAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UBasicAttributeSet::UBasicAttributeSet()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
	Stamina = 100.0f;
	MaxStamina = 100.0f;
}

void UBasicAttributeSet::OnRep_Health(const FGameplayAttributeData& oldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Health, oldHealth);
}

void UBasicAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& oldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, MaxHealth, oldMaxHealth);
}

void UBasicAttributeSet::OnRep_Stamina(const FGameplayAttributeData& oldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Stamina, oldStamina);
}

void UBasicAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& oldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, MaxStamina, oldMaxStamina);
}

void UBasicAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

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

void UBasicAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
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