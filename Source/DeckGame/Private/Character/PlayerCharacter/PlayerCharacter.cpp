// Copyright (c) 2025 Stephen Melnick

#include "Character/PlayerCharacter/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Logging/StructuredLog.h"
#include "Kismet/GameplayStatics.h"

#include "DeckGame.h"
#include "Ability/DeckAbilitySystemComponent.h"
#include "Ability/DeckGameplayTags.h"
#include "Ability/Combos/StateTreeComboComponent.h"
#include "Character/Components/CardDeckComponent.h"
#include "Character/Components/DeckInputComponent.h"
#include "Character/Components/DeckMovementComponent.h"
#include "Character/Components/MeleeComponent.h"
#include "Character/DeckPlayerState.h"
#include "Input/DeckInputConfig.h"


APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	ADeckCharacter(ObjectInitializer.SetDefaultSubobjectClass(ACharacter::CharacterMovementComponentName, UDeckMovementComponent::StaticClass()))
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
	InteractionVolume->ComponentTags.Add("Interaction Volume");

	Melee = CreateDefaultSubobject<UMeleeComponent>(TEXT("Melee"));
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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
	if (ADeckPlayerState* DeckPlayerState = Cast<ADeckPlayerState>(GetPlayerState()))
	{
		if (UStateTreeComboComponent* Combo = DeckPlayerState->GetComboComponent())
		{
			DeckInputComponent->BindAbilityActions(InputConfig, Combo, &UStateTreeComboComponent::Input_AbilityInputTagPressed,
				&UStateTreeComboComponent::Input_AbilityInputTagReleased, BindHandles);
		}

		if (UCardDeckComponent* CardDeckComponent = DeckPlayerState->GetCardDeckComponent())
		{
			DeckInputComponent->BindAbilityActions(InputConfig, CardDeckComponent, &UCardDeckComponent::Input_AbilityInputTagPressed,
				&UCardDeckComponent::Input_AbilityInputTagReleased, BindHandles);
		}
	}
	
	DeckInputComponent->BindNativeAction(InputConfig, DeckGameplayTags::InputTag_Move,
		ETriggerEvent::Triggered, this, &ThisClass::Move, /*bLogIfNotFound=*/ false);
	DeckInputComponent->BindNativeAction(InputConfig, DeckGameplayTags::InputTag_Look,
		ETriggerEvent::Triggered, this, &ThisClass::Look, /*bLogIfNotFound=*/ false);
	DeckInputComponent->BindNativeAction(InputConfig, DeckGameplayTags::InputTag_Scroll,
		ETriggerEvent::Triggered, this, &ThisClass::Scroll, false);

	
	DeckInputComponent->BindNativeAction(InputConfig, DeckGameplayTags::InputTag_Slot_Select,
		ETriggerEvent::Started, this, &ThisClass::Select, true);
	DeckInputComponent->BindNativeAction(InputConfig, DeckGameplayTags::InputTag_Slot_Select,
		ETriggerEvent::Completed, this, &ThisClass::Select, true);
	
	DeckInputComponent->BindNativeAction(InputConfig, DeckGameplayTags::InputTag_Slot_0_Select,
		ETriggerEvent::Started, this, &ThisClass::SelectSlot0, true);
	DeckInputComponent->BindNativeAction(InputConfig, DeckGameplayTags::InputTag_Slot_1_Select,
		ETriggerEvent::Started, this, &ThisClass::SelectSlot1, true);
	DeckInputComponent->BindNativeAction(InputConfig, DeckGameplayTags::InputTag_Slot_2_Select,
		ETriggerEvent::Started, this, &ThisClass::SelectSlot2, true);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ADeckPlayerState* DeckPlayerState = NewController->GetPlayerState<ADeckPlayerState>();
	if (!DeckPlayerState)
	{
		UE_LOGFMT(LogDeckGame, Warning, "Player controller did not have a valid Deck Player State.");
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

void APlayerCharacter::Scroll(const FInputActionInstance& Instance)
{
	float Input = Instance.GetValue().GetMagnitude();
	if (UCardDeckComponent* CardDeckComponent = GetCardDeckComponent())
	{
		CardDeckComponent->ChangeSelectedCard(Input > 0 ? 1 : -1);
	}
}

void APlayerCharacter::Select(const FInputActionInstance& Instance)
{
	if (UCardDeckComponent* CardDeckComponent = GetCardDeckComponent())
	{
		switch (Instance.GetTriggerEvent())
		{
		case ETriggerEvent::Started:
			CardDeckComponent->SetSelectionMode(true);
			break;
		case ETriggerEvent::Completed:
			CardDeckComponent->SetSelectionMode(false);
			break;
		default:
			break;
		}
	}
}

void APlayerCharacter::SelectSlot0(const FInputActionInstance& Instance)
{
	SendSelectionEvent(0);
}

void APlayerCharacter::SelectSlot1(const FInputActionInstance& Instance)
{
	SendSelectionEvent(1);
}

void APlayerCharacter::SelectSlot2(const FInputActionInstance& Instance)
{
	SendSelectionEvent(2);
}

void APlayerCharacter::SendSelectionEvent(uint8 Index)
{
	FGameplayEventData Payload;
	Payload.EventTag = DeckGameplayTags::GameplayEvent_AbilityTrigger_SelectSlot;
	Payload.Instigator = GetPlayerState();
	Payload.Target = GetPlayerState();
	Payload.EventMagnitude = Index;

	GetAbilitySystemComponent()->HandleGameplayEvent(Payload.EventTag, &Payload);
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

void APlayerCharacter::SetDeckTeam(EDeckTeam InTeam)
{
	GetPlayerStateChecked<ADeckPlayerState>()->SetDeckTeam(InTeam);
}

EDeckTeam APlayerCharacter::GetDeckTeam() const
{
	return GetPlayerStateChecked<ADeckPlayerState>()->GetDeckTeam();
}

USphereComponent* APlayerCharacter::GetInteractionVolume() const
{
	return InteractionVolume;
}

bool APlayerCharacter::CanJump() const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC &&
		(ASC->HasMatchingGameplayTag(DeckGameplayTags::GameplayEffect_Stun) || ASC->HasMatchingGameplayTag(DeckGameplayTags::GameplayEffect_Frozen)))
	{
		return false;
	}
	
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
