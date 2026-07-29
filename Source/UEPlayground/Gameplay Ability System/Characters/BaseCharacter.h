#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "BaseCharacter.generated.h"

UCLASS()
class UEPLAYGROUND_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();																								// Constructor
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UAbilitySystemComponent* AbilitySystemComponent;																// Ability System Component reference
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	class UBasicAttributeSet* BasicAttributeSet;																	// Basic Attribute Set reference

protected:
	virtual void BeginPlay() override;																				// Called when the game starts or when spawned
	virtual void PossessedBy(AController* NewController) override;													// Called when the character is possessed by a controller
	virtual void OnRep_PlayerState() override;																		// Called on clients when the PlayerState is replicated
	virtual void OnDeadTagChanged(const FGameplayTag CallbackTag, int32 NewCount);									// Called when the 'Dead' Tag is changed

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	EGameplayEffectReplicationMode ASCReplicationMode = EGameplayEffectReplicationMode::Mixed;						// Replication mode for the Ability System Component

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;														// An array of starting Gameplay Ability classes

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void HandleDeath();																								// 
	
public:	
	virtual void Tick(float DeltaTime) override;																	// Called every frame
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;					// Called to bind functionality to input
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;									// Returns the Ability System Component reference
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	TArray<FGameplayAbilitySpecHandle> GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant);		// Grants a list of gameplay abilities and returns their handles
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove);								// Clears a list of previously granted gameplay abilities
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void SendAbilitiesChangedEvent();																				// Broadcasts a gameplay event when the character's ability set changes
};