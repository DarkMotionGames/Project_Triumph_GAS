// © 2024 DMG. All rights reserved.


#include "Player/PlayerControllerBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "GameFramework/Character.h"
#include "Input/MainInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/WidgetComponent/DamageTextComponent.h"


APlayerControllerBase::APlayerControllerBase()
{
	bReplicates = true;
}

void APlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void APlayerControllerBase::AbilityInputTagPressed(FGameplayTag InputTag)
{
		
}

void APlayerControllerBase::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponentBase() == nullptr)
	{
		return;
	}
	GetAbilitySystemComponentBase()->AbilityInputTagReleased(InputTag);
}

void APlayerControllerBase::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponentBase() == nullptr)
	{
		return;
	}
	GetAbilitySystemComponentBase()->AbilityInputTagHeld(InputTag);
}

UAbilitySystemComponentBase* APlayerControllerBase::GetAbilitySystemComponentBase()
{
	if (AbilitySystemComponentBase == nullptr)
	{
		AbilitySystemComponentBase = Cast<UAbilitySystemComponentBase>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AbilitySystemComponentBase;
}



void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	check(PlayerContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(PlayerContext, 0);
	}
}



void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UMainInputComponent* MainInputComponent = CastChecked<UMainInputComponent>(InputComponent);

	MainInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerControllerBase::Move);
	MainInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerControllerBase::Look);
	MainInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerControllerBase::Jump);
	MainInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerControllerBase::StopJump);
	MainInputComponent->BindAction(TabAction, ETriggerEvent::Triggered, this, &APlayerControllerBase::OpenTabMenu);

	MainInputComponent->BindAbilityActions(InputConfig, this, &APlayerControllerBase::AbilityInputTagPressed, &APlayerControllerBase::AbilityInputTagReleased, &APlayerControllerBase::AbilityInputTagHeld);
	
}


void APlayerControllerBase::AttachDamageTextToCharacter(USceneComponent* DamageText, const ACharacter* TargetCharacter, const FName SocketName)
{
	// Ensure that TargetCharacter has a valid mesh to attach to
	if (TargetCharacter && TargetCharacter->GetMesh())
	{
		// Attaching DamageText to the specified socket
		DamageText->AttachToComponent(TargetCharacter->GetMesh(),
			FAttachmentTransformRules::KeepRelativeTransform, 
			SocketName                                
		);
	}
}

void APlayerControllerBase::ShowDamageNumber_Implementation(const float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		const FName DamageSocketName = FName(TEXT("HeadSocket")); 
		AttachDamageTextToCharacter(DamageText, TargetCharacter, DamageSocketName);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}	
}

void APlayerControllerBase::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void APlayerControllerBase::Look(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
	}
}

void APlayerControllerBase::Jump()
{
	TObjectPtr<ACharacter> ControlledPlayer = GetPawn<ACharacter>();
	if (!IsValid(ControlledPlayer))
		return;
	ControlledPlayer->Jump();
}

void APlayerControllerBase::StopJump()
{
	TObjectPtr<ACharacter> ControlledPlayer = GetPawn<ACharacter>();
	if (!IsValid(ControlledPlayer))
		return;
	ControlledPlayer->StopJumping();
}

void APlayerControllerBase::OpenTabMenu()
{
	TabKeyPressedDelegate.Broadcast();
}

void APlayerControllerBase::CursorTrace()
{
	FHitResult CursorHitResult;
	GetHitResultUnderCursor(ECC_Visibility,false, CursorHitResult);
	if (!CursorHitResult.bBlockingHit)
	{
		return;
	}
	LastHitActor = CurrentHitActor;
	CurrentHitActor = CursorHitResult.GetActor();
	if (LastHitActor.GetInterface() == nullptr)
	{
		if (CurrentHitActor.GetInterface() != nullptr)
		{
			// Case B
			CurrentHitActor->HighlightActor();
		}
		else
		{
			// Case A - both are null, do nothing
		}
	}
	else // LastActor is valid
	{
		if (CurrentHitActor.GetInterface() == nullptr)
		{
			// Case C
			LastHitActor->UnHighlightActor();
		}
		else // both actors are valid
		{
			if (LastHitActor.GetInterface() != CurrentHitActor.GetInterface())
			{
				// Case D
				LastHitActor->UnHighlightActor();
				CurrentHitActor->HighlightActor();
			}
			else
			{
				// Case E - do nothing
			}
		}
	}
}