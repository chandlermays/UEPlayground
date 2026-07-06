#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PlaygroundAbilitySystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UEPLAYGROUND_API UPlaygroundAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPlaygroundAbilitySystemComponent();																							// Constructor

protected:
	virtual void BeginPlay() override;																								// Called when the game starts or when spawned

	virtual void OnRep_ActivateAbilities() override;																				// Called on clients when the activatable abilities array is replicated
	
	TArray<FGameplayAbilitySpec> LastActivatableAbilities;																			// An array of the last activatable abilities
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		// Called every frame
};