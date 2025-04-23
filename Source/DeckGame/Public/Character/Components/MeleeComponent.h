// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VisualLogger/VisualLoggerDebugSnapshotInterface.h"
#include "MeleeComponent.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogMelee, Log, All);

UENUM(BlueprintType)
enum EMeleeHitboxShape : uint8
{
	Box,
	Sphere,
	Capsule
};

UENUM(BlueprintType)
enum EMeleeHitboxType : uint8
{
	Offensive,
	Shield
};

USTRUCT(BlueprintType)
struct FMeleeHitboxSpawnParameters
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMeleeHitboxShape> Shape = Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMeleeHitboxType> Type = Offensive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Shape == 1 || Shape == 2", EditConditionHides, ClampMin = 0.0f))
	float Radius = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Shape == 2", EditConditionHides, ClampMin = 0.0f))
	float HalfHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Shape == 0", EditConditionHides, ClampMin = 0.0f))
	FVector BoxExtents = FVector::ZeroVector;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DECKGAME_API UMeleeComponent : public UActorComponent, public IVisualLoggerDebugSnapshotInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = true))
	FCollisionProfileName CollisionProfile;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = true))
	FCollisionProfileName ShieldCollisionProfile;

	/// The collision channel to check for shields when verifying if an attack landed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = true))
	TEnumAsByte<ECollisionChannel> ShieldCollisionChannel = ECC_GameTraceChannel1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = true))
	bool bShowHitboxes = false;

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnActorHit, AActor*)
	FOnActorHit OnActorHit;

	UMeleeComponent();

	// @TODO: finish writing handle class that can be used in place of a pointer type
	UShapeComponent* SpawnHitbox(FMeleeHitboxSpawnParameters SpawnParameters);

	void DestroyHitbox(UShapeComponent* Hitbox);

	// Function for binding to dynamic delegate for OnComponentBeginOverlap
	// Forwards to our OnActorHit Delegate
	UFUNCTION()
	void OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// Called when an attack is blocked by a shield hitbox
	/// @param SourceComponent MeleeComponent of the Actor that attacked us
	/// @param ShieldHitbox Hitbox that blocked the attempted hit
	/// @param BlockHit HitResult from the attack line trace
	void OnAttackBlocked(UMeleeComponent* SourceComponent, UPrimitiveComponent* ShieldHitbox, FHitResult BlockHit);
	
	void DestroyAllHitboxes();
	
private:
	
#if ENABLE_VISUAL_LOG
	void GrabDebugSnapshot(FVisualLogEntry* Snapshot) const override;
#endif // ENABLE_VISUAL_LOG

	template <typename FmtType, typename... Types>
	void LogCollisionShape(UPrimitiveComponent* Primitive, const FColor Color, const FmtType& Fmt, Types... Args)
	{
		const FCollisionShape CollisionShape = Primitive->GetCollisionShape();
		switch (CollisionShape.ShapeType)
		{
		case ECollisionShape::Sphere:
			{
				UE_VLOG_SPHERE(this, LogMelee, Display, Primitive->GetComponentLocation(), CollisionShape.GetSphereRadius(), FColor::Blue,
					Fmt, Args...);
				break;
			}
		case ECollisionShape::Capsule:
			{
				UE_VLOG_CAPSULE(this, LogMelee, Display, Primitive->GetComponentLocation() - Primitive->GetUpVector() * CollisionShape.GetCapsuleHalfHeight(),
					CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), Primitive->GetComponentQuat(), FColor::Blue,
					Fmt, Args...);
				break;
			}
		case ECollisionShape::Box:
			{
				if (FVisualLogger::IsRecording())
				{
					FVisualLogger::BoxLogf(this, LogMelee, ELogVerbosity::Display, FBox(-CollisionShape.GetBox(), CollisionShape.GetBox()),
						Primitive->GetComponentTransform().ToMatrixNoScale(), FColor::Blue, false, Fmt, Args...);
				}
				break;
			}
		default:
			break;
		}
	}

	static FColor GetHitboxDefaultColor(EMeleeHitboxType Type);
	
	TArray<UShapeComponent*> SpawnedHitboxes;

};