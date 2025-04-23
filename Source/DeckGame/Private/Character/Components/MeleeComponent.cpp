// Copyright (c) 2024 Stephen Melnick


#include "Character/Components/MeleeComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY(LogMelee);

UMeleeComponent::UMeleeComponent()
{
}


UShapeComponent* UMeleeComponent::SpawnHitbox(FMeleeHitboxSpawnParameters SpawnParameters)
{
	USkeletalMeshComponent* SkeletalMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	if (!SkeletalMesh)
	{
		return nullptr;
	}
	UShapeComponent* SpawnedShape = nullptr;

	// Since we're using a TEnumAsByte, even though all the backing enum's cases are covered,
	// it will ask for a default case...
	
	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	switch (SpawnParameters.Shape)
	{
	case Box:
		{
			UBoxComponent* Box = NewObject<UBoxComponent>(this);
			Box->SetBoxExtent(SpawnParameters.BoxExtents);
			SpawnedShape = Box;
			break;
		}
	case Capsule:
		{
			UCapsuleComponent* Capsule = NewObject<UCapsuleComponent>(this);
			Capsule->SetCapsuleRadius(SpawnParameters.Radius);
			Capsule->SetCapsuleHalfHeight(SpawnParameters.HalfHeight);
			SpawnedShape = Capsule;
			break;
		}
	case Sphere:
		{
			USphereComponent* Sphere = NewObject<USphereComponent>(this);
			Sphere->SetSphereRadius(SpawnParameters.Radius);
			SpawnedShape = Sphere;
			break;
		}
	}
	SpawnedShape->SetGenerateOverlapEvents(true);
	FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
	SpawnedShape->AttachToComponent(SkeletalMesh, AttachmentTransformRules);
	SpawnedShape->SetRelativeLocationAndRotation(SpawnParameters.Position, SpawnParameters.Rotation);

	FCollisionProfileName CollisionProfileName;

	// Same as before with the shape
	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	switch (SpawnParameters.Type)
	{
	case Offensive:
		{
			CollisionProfileName = CollisionProfile;
			SpawnedShape->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnHitboxOverlap);
			break;
		}
	case Shield:
		{
			CollisionProfileName = ShieldCollisionProfile;
			break;
		}
	}
	
	// Set the collision type, but make the object type dynamic
	SpawnedShape->SetCollisionProfileName(CollisionProfileName.Name);
	SpawnedShape->SetCollisionObjectType(ECC_WorldDynamic);
	// Even though this ignores moving, it also works for the registration function later
	SpawnedShape->IgnoreActorWhenMoving(GetOwner(), true);
	SpawnedShape->SetHiddenInGame(false);
	SpawnedShape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SpawnedShape->SetVisibility(bShowHitboxes);
	SpawnedShape->ShapeColor = GetHitboxDefaultColor(SpawnParameters.Type);

	// Overlaps are automatically updated here
	SpawnedShape->RegisterComponentWithWorld(GetWorld());
	// TOverlapArrayView Overlaps;

	// TArray<FOverlapResult> OverlapResults;
	// GetWorld()->OverlapMultiByProfile(OverlapResults, SpawnedShape->GetComponentLocation(), SpawnedShape->GetComponentQuat(), SpawnedShape->GetCollisionProfileName(), SpawnedShape->GetCollisionShape());
	// UE_LOG(LogTemp, Display, TEXT("%i overlaps found when using world overlap test."), OverlapResults.Num());

	SpawnedHitboxes.AddUnique(SpawnedShape);
	return SpawnedShape;
}

void UMeleeComponent::DestroyHitbox(UShapeComponent* Hitbox)
{
	int32 ShapeIndex;
	if (SpawnedHitboxes.Find(Hitbox, ShapeIndex))
	{
		SpawnedHitboxes.RemoveAt(ShapeIndex);
		Hitbox->DestroyComponent();
	}
}

void UMeleeComponent::OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TArray<FHitResult> Overlaps;
	const FVector StartLocation = OverlappedComponent->GetComponentLocation();
	const FVector EndLocation = OtherActor->GetActorLocation();
	GetWorld()->LineTraceMultiByChannel(Overlaps, StartLocation, EndLocation, ShieldCollisionChannel);

	
	UE_VLOG_ARROW(this, LogMelee, Display, StartLocation, EndLocation, FColor::Red, TEXT("Sweeping Melee hitbox."));
	LogCollisionShape(OverlappedComponent, FColor::Red, TEXT("Hitbox '%s' collided with enemy."), *GetNameSafe(OverlappedComponent));
	if (ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor))
	{
		if (UCapsuleComponent* EnemyCapsule = OtherCharacter->GetCapsuleComponent())
		{
			UE_VLOG_CAPSULE(this, LogMelee, Display, EnemyCapsule->GetComponentLocation() - FVector(0.0f, 0.0f, EnemyCapsule->GetScaledCapsuleHalfHeight()), EnemyCapsule->GetScaledCapsuleHalfHeight(), EnemyCapsule->GetScaledCapsuleRadius(),
				EnemyCapsule->GetComponentQuat(), FColor::Red, TEXT("Actor '%s' hit '%s'"), *GetNameSafe(GetOwner()), *GetNameSafe(OtherActor));
		}
	}

	for (FHitResult& Overlap : Overlaps)
	{
		// Ignore shields that don't belong to the actor we're trying to attack
		if (Overlap.GetActor() != OtherActor)
		{
			continue;
		}
		
		if (UMeleeComponent* MeleeComponent = Overlap.GetActor()->FindComponentByClass<UMeleeComponent>())
		{
			MeleeComponent->OnAttackBlocked(this, OverlappedComponent, SweepResult);
			LogCollisionShape(Overlap.GetComponent(), FColor::Blue, TEXT("Hit blocked by actor '%s'"), *GetNameSafe(OtherActor));
		}
		return;
	}
	
	OnActorHit.Broadcast(OtherActor);
}

void UMeleeComponent::OnAttackBlocked(UMeleeComponent* SourceComponent, UPrimitiveComponent* ShieldHitbox,
	FHitResult BlockHit)
{
}

void UMeleeComponent::DestroyAllHitboxes()
{
	for (UShapeComponent* Hitbox : SpawnedHitboxes)
	{
		Hitbox->DestroyComponent();
	}

	SpawnedHitboxes.Reset();
}

FColor UMeleeComponent::GetHitboxDefaultColor(EMeleeHitboxType Type)
{
	switch (Type)
	{
	case Offensive:
		return FColor::Red;
	case Shield:
		return FColor::Blue;
	default:
		return FColor::Cyan;
	}
}


#if ENABLE_VISUAL_LOG
void UMeleeComponent::GrabDebugSnapshot(FVisualLogEntry* Snapshot) const
{
	IVisualLoggerDebugSnapshotInterface::GrabDebugSnapshot(Snapshot);
	
	const int32 CatIndex = Snapshot->Status.AddZeroed();
	FVisualLogStatusCategory& PlaceableCategory = Snapshot->Status[CatIndex];
	PlaceableCategory.Category = TEXT("Melee Combat");
}
#endif
