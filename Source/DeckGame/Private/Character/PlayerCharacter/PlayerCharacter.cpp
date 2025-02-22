// Copyright (c) 2025 Stephen Melnick

#include "Character/PlayerCharacter/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/SphereComponent.h"
#include "Logging/StructuredLog.h"
#include "Kismet/GameplayStatics.h"

#include "DeckGame.h"
#include "Ability/DeckAbilitySystemComponent.h"
#include "Ability/DeckGameplayTags.h"
#include "Character/Components/CardDeckComponent.h"
#include "Character/Components/DeckInputComponent.h"
#include "Character/Components/DeckMovementComponent.h"
#include "Character/Components/MeleeComponent.h"
#include "Character/DeckPlayerState.h"
#include "Input/DeckInputConfig.h"


APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	ACharacter(ObjectInitializer.SetDefaultSubobjectClass(ACharacter::CharacterMovementComponentName, UDeckMovementComponent::StaticClass()))
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::InteractVolumeBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::InteractVolumeEndOverlap);

	Melee = CreateDefaultSubobject<UMeleeComponent>(TEXT("Melee"));
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetActorLocation().Z <= -1000.0f)
	{
		//TeleportToLastSafeLocation();
		TeleportToStart();
	}
}

void APlayerCharacter::InteractVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void APlayerCharacter::InteractVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	BindActions(PlayerInputComponent);
}

void APlayerCharacter::BindActions(UInputComponent* PlayerInputComponent)
{
	if (!InputConfig)
	{
		UE_LOGFMT(LogDeckGame, Warning, "Unable to bind actions to input component on pawn {Name}. InputConfig is not set.", GetName());
		return;
	}

	UDeckInputComponent* DeckInputComponent = Cast<UDeckInputComponent>(PlayerInputComponent);
	if (!DeckInputComponent)
	{
		return;
	}

	TArray<uint32> BindHandles;
	DeckInputComponent->BindAbilityActions(InputConfig, this, &APlayerCharacter::Input_AbilityInputTagPressed, &APlayerCharacter::Input_AbilityInputTagReleased, BindHandles);

	DeckInputComponent->BindNativeAction(InputConfig, DeckGameplayTags::InputTag_Move,
		ETriggerEvent::Triggered, this, &ThisClass::Move, /*bLogIfNotFound=*/ false);
	DeckInputComponent->BindNativeAction(InputConfig, DeckGameplayTags::InputTag_Look,
		ETriggerEvent::Triggered, this, &ThisClass::Look, /*bLogIfNotFound=*/ false);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ADeckPlayerState* DeckPlayerState = NewController->GetPlayerState<ADeckPlayerState>();
	if (!DeckPlayerState)
	{
		return;
	}

	UDeckAbilitySystemComponent* ASC = DeckPlayerState->GetDeckAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	ASC->SetAvatarActor(this);
	if (UDeckMovementComponent* DeckMovement = Cast<UDeckMovementComponent>(GetMovementComponent()))
	{
		DeckMovement->SetASC(ASC);
	}
}

void APlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	UDeckAbilitySystemComponent* DeckASC = GetDeckAbilitySystemComponent();
	if (!DeckASC)
	{
		UE_LOGFMT(LogDeckGame, Warning, "Unable to activate ability input in APlayerCharacter {Name}. Unable to access AbilitySystemComponent from player state.", ("Name", GetFName()));
		return;
	}
	DeckASC->AbilityInputTagPressed(InputTag);
}

void APlayerCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	UDeckAbilitySystemComponent* DeckASC = GetDeckAbilitySystemComponent();
	if (!DeckASC)
	{
		UE_LOGFMT(LogDeckGame, Warning, "Unable to activate ability input in APlayerCharacter {Name}. Unable to access AbilitySystemComponent from player state.", ("Name", GetFName()));
		return;
	}
	DeckASC->AbilityInputTagReleased(InputTag);
}

void APlayerCharacter::Move(const FInputActionInstance& Instance)
{
	FVector2D Input = Instance.GetValue().Get<FVector2D>();
	if (Controller && !Input.IsNearlyZero())
	{
		//const float Rotation = Controller->GetControlRotation().Yaw;
		const float Rotation = FollowCamera->GetComponentRotation().Yaw + 90.0f;
		const FVector2D RotatedInput = Input.GetRotated(-Rotation);
		const FVector Movement = FVector(RotatedInput.X, -RotatedInput.Y, 0.0);
		AddMovementInput(Movement);
	}
}

void APlayerCharacter::Look(const FInputActionInstance& Instance)
{
	FVector2D Input = Instance.GetValue().Get<FVector2D>();
	if (Controller && !Input.IsNearlyZero())
	{
		if (bTopDownCamera)
		{
			RotateTopDownCamera(Input);
		}
		else
		{
			RotateOTSCamera(Input);
		}
	}
}

void APlayerCharacter::RotateTopDownCamera(FVector2D Input)
{
	const FVector2D InputNormalized = Input.GetSafeNormal();
	const float InputRotation = FMath::Atan2(InputNormalized.Y, InputNormalized.X);
	const float CameraRotation = FollowCamera->GetComponentRotation().Yaw + 90.0f;

	Controller->SetControlRotation(FRotator(0.0f, FMath::RadiansToDegrees(InputRotation) + CameraRotation, 0.0f));
}

void APlayerCharacter::RotateOTSCamera(FVector2D Input)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	AddControllerPitchInput(Input.Y * World->GetDeltaSeconds() * 150.0f);
	AddControllerYawInput(Input.X * World->GetDeltaSeconds() * 300.0f);
}

UCardDeckComponent* APlayerCharacter::GetCardDeckComponent() const
{
	APlayerState* OwningPlayerState = GetPlayerState();
	if (!OwningPlayerState)
	{
		return nullptr;
	}

	UCardDeckComponent* CardDeck = OwningPlayerState->GetComponentByClass<UCardDeckComponent>();
	if (!CardDeck)
	{
		return nullptr;
	}

	return CardDeck;
}

USphereComponent* APlayerCharacter::GetInteractionVolume() const
{
	return InteractionVolume;
}

void APlayerCharacter::TeleportToLastSafeLocation()
{
	GetMovementComponent()->Velocity = FVector();
	TeleportTo(LastSafeLocation, GetActorRotation());
}

bool APlayerCharacter::CanJump() const
{
	return CanJumpInternal();
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return GetDeckAbilitySystemComponent();
}

UDeckAbilitySystemComponent* APlayerCharacter::GetDeckAbilitySystemComponent () const
{
	APlayerState* State = GetPlayerState();
	if (!State)
	{
		// Don't log here? Expect whoever is calling the method to handle nullptr instead
		// UE_LOGFMT(LogDeckGame, Warning, "Unable to get ASC in APlayerCharacter {Name}. The player state is invalid.", GetName());
		return nullptr;
	}
	ADeckPlayerState* DeckPlayerState = Cast<ADeckPlayerState>(State);
	if (!DeckPlayerState)
	{
		UE_LOGFMT(LogDeckGame, Warning, "Unable to get ASC in APlayerCharacter {Name}. The player state is an unexpected type. It must be of type APlayerCharacterPlayerState.", GetName());
		return nullptr;
	}
	return DeckPlayerState->GetDeckAbilitySystemComponent();
}

void APlayerCharacter::TeleportToStart()
{
	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(GetWorld());
	if (!GameModeBase)
	{
		return;
	}

	AController* MyPlayerController = GetController();
	if (!MyPlayerController)
	{
		return;
	}

	if (AActor* StartActor = GameModeBase->FindPlayerStart(MyPlayerController))
	{
		TeleportTo(StartActor->GetActorLocation(), GetActorRotation());
	}
}
