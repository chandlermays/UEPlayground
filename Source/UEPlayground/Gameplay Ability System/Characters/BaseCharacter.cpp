#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

/*------------------------------------------------------------
| --- Constructor: Sets default values for BaseCharacter --- |
------------------------------------------------------------*/
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Add the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(ASCReplicationMode);
	
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