#include "BasicAttributeSet.h"
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
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Health, oldMaxHealth);
}

void UBasicAttributeSet::OnRep_Stamina(const FGameplayAttributeData& oldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Health, oldStamina);
}

void UBasicAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& oldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Health, oldMaxStamina);
}

void UBasicAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}