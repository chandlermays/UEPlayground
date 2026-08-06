#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PlaygroundGameplayAbility.generated.h"

UCLASS()
class UEPLAYGROUND_API UPlaygroundGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPlaygroundGameplayAbility();															// Constructor
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool ShowInAbilityContainer = false;													// Whether to show this ability in the ability container UI
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText AbilityDisplayName;																// Display name for the ability in the UI

private:
	UFUNCTION(BlueprintCallable, Category = "Helpers")
	bool HasPC() const;																		// Helper function to check if the ability has a valid PlayerController
};