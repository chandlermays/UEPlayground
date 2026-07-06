#include "BaseCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UEPlayground/Gameplay Ability System/PlaygroundAbilitySystemComponent.h"
#include "UEPlayground/Gameplay Ability System/AttributeSets/BasicAttributeSet.h"

/*------------------------------------------------------------
| --- Constructor: Sets default values for BaseCharacter --- |
------------------------------------------------------------*/
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Add the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UPlaygroundAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(ASCReplicationMode);
	
	// Add the Basic Attribute Set
	BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributeSet"));
	
	// Character Movement Component configuration
	/*-----------------------------------------*/
	
	// Set the size for the collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.0f, 90.0f);
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
}

/*----------------------------------------------------------------
| --- BeginPlay: Called when the game starts or when spawned --- |
----------------------------------------------------------------*/
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

/*-----------------------------------------------------------------------------
| --- PossessedBy: Called when the character is possessed by a controller --- |
-----------------------------------------------------------------------------*/
void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GrantAbilities(StartingAbilities);
	}
}

/*---------------------------------------------------------------------------------
| --- OnRep_PlayerState: Called on clients when the PlayerState is replicated --- |
---------------------------------------------------------------------------------*/
void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

/*----------------------------------
| --- Tick: Called every frame --- |
----------------------------------*/
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*--------------------------------------------------------------------------
| --- SetupPlayerInputComponent: Called to bind functionality to input --- |
--------------------------------------------------------------------------*/
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/*-----------------------------------------------------------------------------------
| --- GetAbilitySystemComponent: Returns the Ability System Component reference --- |
-----------------------------------------------------------------------------------*/
UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

/*---------------------------------------------------------------------------------------
| --- GrantAbilities: Grants a list of gameplay abilities and returns their handles --- |
---------------------------------------------------------------------------------------*/
TArray<FGameplayAbilitySpecHandle> ABaseCharacter::GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant)
{
	if (!AbilitySystemComponent || !HasAuthority())
	{
		// return empty array if null - OR - not on the server
		return TArray<FGameplayAbilitySpecHandle>();
	}
	
	TArray<FGameplayAbilitySpecHandle> AbilityHandles;
	for (TSubclassOf<UGameplayAbility> Ability : AbilitiesToGrant)
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, -1, this));
		AbilityHandles.Add(SpecHandle);
	}
	
	SendAbilitiesChangedEvent();
	
	return AbilityHandles;
}

/*---------------------------------------------------------------------------------
| --- RemoveAbilities: Clears a list of previously granted gameplay abilities --- |
---------------------------------------------------------------------------------*/
void ABaseCharacter::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove)
{
	if (!AbilitySystemComponent || !HasAuthority())
	{
		// return if null - OR - not on the server
		return;
	}
	
	for (FGameplayAbilitySpecHandle AbilityHandle : AbilityHandlesToRemove)
	{
		AbilitySystemComponent->ClearAbility(AbilityHandle);
	}
	
	SendAbilitiesChangedEvent();
}

/*---------------------------------------------------------------------------------------------------------
| --- SendAbilitiesChangedEvent: Broadcasts a gameplay event when the character's ability set changes --- |
---------------------------------------------------------------------------------------------------------*/
void ABaseCharacter::SendAbilitiesChangedEvent()
{
	FGameplayEventData EventData;
	EventData.EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Abilities.Changed"));
	EventData.Instigator = this;
	EventData.Target = this;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);
}