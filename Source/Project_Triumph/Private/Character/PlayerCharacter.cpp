// © 2024 DMG. All rights reserved.


#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayerControllerBase.h"
#include "Player/PlayerStateBase.h"
#include "UI/HUD/MainHUD.h"

APlayerCharacter::APlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 390.0f, 0.0f); // ...at this rotation rate
	
	GetMesh()->SetGenerateOverlapEvents(true);
	
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// ~ Init Ability actor info for the Server
	InitializeAbilityActorInfo();
	AddCharacterAbilities();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Fixes issue where skills (such as fire bolt) is not firing on the server as notify event is not being received.
	// Enabling auto-flush ensures that network packets are immediately sent to the recipient without additional delay
	if (UNetConnection* Connection = GetNetConnection())
	{
		Connection->SetAutoFlush(true);
	}
	// ~ Init Ability actor info for the Client
	InitializeAbilityActorInfo();
}

int32 APlayerCharacter::GetPlayerLevel()
{
	const APlayerStateBase* GamePlayerState = GetPlayerState<APlayerStateBase>();
	check(GamePlayerState);
	return GamePlayerState->GetPlayerLevel();
}

void APlayerCharacter::InitializeAbilityActorInfo()
{
	APlayerStateBase* GamePlayerState = GetPlayerState<APlayerStateBase>();
	check(GamePlayerState);
	GamePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(GamePlayerState, this);
	Cast<UAbilitySystemComponentBase>(GamePlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	GameAbilitySystemComponent = GamePlayerState->GetAbilitySystemComponent();
	GameAttributeSet = GamePlayerState->GetAttributeSet();

	if (APlayerControllerBase* PlayerControllerBase = Cast<APlayerControllerBase>(GetController()))
	{
		if (AMainHUD* MainHUD = Cast<AMainHUD>(PlayerControllerBase->GetHUD()))
		{
			MainHUD->InitOverlay(PlayerControllerBase, GamePlayerState, GameAbilitySystemComponent, GameAttributeSet);
		}
	}
	InitializeDefaultAttributes();
}
