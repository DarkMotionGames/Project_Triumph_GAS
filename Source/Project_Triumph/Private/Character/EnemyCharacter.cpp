// © 2024 DMG. All rights reserved.


#include "Character/EnemyCharacter.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "AbilitySystem/MainAbilitySystemLibrary.h"
#include "AbilitySystem/AttributeSetBase.h"
#include "Components/WidgetComponent.h"
#include "Project_Triumph/Project_Triumph.h"
#include "UI/Widget/UserWidgetBase.h"
#include "MainGameplayTags.h"
#include "AI/MainAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GameAbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponentBase>("AbilitySystemComponent");
	GameAbilitySystemComponent->SetIsReplicated(true);
	GameAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	GameAttributeSet = CreateDefaultSubobject<UAttributeSetBase>("AttributeSet");
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->MarkRenderStateDirty();
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority())
	{
		return;
	}
	
	MainAIController = Cast<AMainAIController>(NewController);
	MainAIController->GetBlackboardComponent()->InitializeBlackboard(*AIBehaviorTree->BlackboardAsset);
	MainAIController->RunBehaviorTree(AIBehaviorTree);
	MainAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	MainAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitializeAbilityActorInfo();
	if (HasAuthority())
	{
		UMainAbilitySystemLibrary::GiveStartupAbilities(this, GameAbilitySystemComponent, CharacterClass);
	}
	
	if (UUserWidgetBase* UserWidgetBase = Cast<UUserWidgetBase>(HealthBar->GetUserWidgetObject()))
	{
		UserWidgetBase->SetWidgetController(this);
	}

	if (const UAttributeSetBase* AttributeSetBase = Cast<UAttributeSetBase>(GameAttributeSet))
	{
		GameAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		GameAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		GameAbilitySystemComponent->RegisterGameplayTagEvent(FMainGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &AEnemyCharacter::HitReactTagChanged
		);
		
		OnHealthChanged.Broadcast(AttributeSetBase->GetHealth());
		OnMaxHealthChanged.Broadcast(AttributeSetBase->GetMaxHealth());
	}
}

void AEnemyCharacter::InitializeAbilityActorInfo()
{
	check(GameAbilitySystemComponent);
	GameAbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAbilitySystemComponentBase>(GameAbilitySystemComponent)->AbilityActorInfoSet();
	
	if (HasAuthority())
	{
		InitializeDefaultAttributes();		
	}
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	UMainAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, GameAbilitySystemComponent);
}

void AEnemyCharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250.f);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(250.f);
	bIsHighlighted = true;
}

void AEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
	bIsHighlighted = false;
}

void AEnemyCharacter::HitReactTagChanged(const FGameplayTag ReactCallbackTag, int32 NewCount)
{
	bIsHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsHitReacting ? 0.f : BaseWalkSpeed;
	if (MainAIController && MainAIController->GetBlackboardComponent())
	{
		MainAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bIsHitReacting);
	}
}


int32 AEnemyCharacter::GetPlayerLevel()
{
	return Level;
}

void AEnemyCharacter::Die()
{
	if (MainAIController && MainAIController->GetBlackboardComponent())
	{
		MainAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	SetLifeSpan(DisappearLifeSpan);
	Super::Die();
}

void AEnemyCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AEnemyCharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AEnemyCharacter::MulticastHandleDeath_Implementation()
{
	HealthBar->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Super::MulticastHandleDeath_Implementation();
}
