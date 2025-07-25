// © 2024 DMG. All rights reserved.


#include "Actor/ProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/MainAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Project_Triumph/Project_Triumph.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	SetLifeSpan(ProjectileLifeSpan);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnSphereOverlap);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopTravelSound, GetRootComponent());
}


void AProjectileBase::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		const FString NetPrefix = GetWorld()->IsNetMode(NM_Client) ? "[CLIENT] " : "[SERVER] ";
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, NetPrefix + TEXT("Play Sound -- Destroyed"));
		ExecuteImpactFXs();
		if (LoopingSoundComponent)
		{
			LoopingSoundComponent->Stop();
		}
		
	}
	Super::Destroyed();
}

void AProjectileBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return;
	}
	if (!UMainAbilitySystemLibrary::IsNotFriendly(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	{
		return;
	}
	if (!bHit)
	{
		// NOTE: We want to execute the FXs in both server and client as soon as possible.
		ExecuteImpactFXs();
		
		if (LoopingSoundComponent)
		{
			LoopingSoundComponent->Stop();
		}
	}
	AActor* ThisOwner = GetOwner();
	// If the Owner is invalid, destroy this projectile without causing damage or executing the VFX/SFX.
	// This handles the case when Destroy() has already being called on the Owner (we currently set
	// a brief lifespan on death for enemy characters), so it's invalid (i.e. either pending kill or null).
	// If we apply a damage GE while the source character (Owner) is invalid, it ends up triggering an exception in
	// UExecCalc_Damage::Execute_Implementation() caused by ExecutionParams.GetSourceAbilitySystemComponent() being null.
	// FIXME: Find a way to allow a projectile with an already destroyed Owner to do damage.
	// Potential solution: From the base character keep track of the projectiles' lifetime, and when the character dies,
	// delay calling Destroy() until all projectiles have been destroyed.
	if (!IsValid(ThisOwner))
	{
		bHit = true;
		Destroy();
		return;
	}
	// Ignore the Owner.
	if (OtherActor == ThisOwner) return;
 
	const FString NetPrefix = GetWorld()->IsNetMode(NM_Client) ? "[CLIENT] " : "[SERVER] ";
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Magenta, NetPrefix + TEXT("Play Sound -- OnBeginOverlap"));
	
	if (HasAuthority())
	{
		// NOTE: DamageEffectSpecHandle should be valid only on the server (we set it there, but also don't replicate it).
		check(DamageEffectSpecHandle.Data);
		UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (TargetAbilitySystemComponent)
		{
			TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
 
		Destroy();
	}
	else
	{
		SetActorHiddenInGame(true);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bHit = true;
	}
}

void AProjectileBase::ExecuteImpactFXs()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
}