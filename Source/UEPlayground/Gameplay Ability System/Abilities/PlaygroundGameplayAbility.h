#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PlaygroundGameplayAbility.generated.h"

UCLASS()
class UEPLAYGROUND_API UPlaygroundGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool ShowInAbilityContainer = false;
};