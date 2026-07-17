#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PlaygroundGameplayAbility.generated.h"

UCLASS()
class UEPLAYGROUND_API UPlaygroundGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPlaygroundGameplayAbility();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool ShowInAbilityContainer = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText AbilityDisplayName;
};