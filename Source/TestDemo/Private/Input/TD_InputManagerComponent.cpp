#include "Input/TD_InputManagerComponent.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Input/TD_InputHandlerHelpers.h"
#include "Input/TD_InputLog.h"
#include "Input/TD_InputSetting.h"
#include "Input/Data/TD_InputSetupDataAsset.h"
#include "Input/TD_InputType.h"

UTD_InputManagerComponent::UTD_InputManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTD_InputManagerComponent::RegisterOwnerAbilitySystemComponent(UAbilitySystemComponent* InVal)
{
	check(InVal);
	OwnerAbilitySystemComponent = InVal;
}

void UTD_InputManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	const TObjectPtr<APawn> OwnerPawn = Cast<APawn>(GetOwner());
	if (true)
	{
		
	}
}

void UTD_InputManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

FVector UTD_InputManagerComponent::GetForwardVector_Implementation() const
{
	FVector Reference = FVector::ZeroVector;
	GetVectorForAxis(EAxis::X, Reference);
	return Reference;
}

FVector UTD_InputManagerComponent::GetRightVector_Implementation() const
{
	FVector Reference = FVector::ZeroVector;
	GetVectorForAxis(EAxis::Y, Reference);
	return Reference;
}

FVector UTD_InputManagerComponent::GetLastInputVector() const
{
	const TObjectPtr<APawn> PawnOwner = GetPawn();
	if (IsValid(PawnOwner))
	{
		//if (PawnOwner->Implements<ULastInputProviderInterface>())
		//{
		//	return ILastInputProviderInterface::Execute_GetLastInputVector(PawnOwner);
		//}

		const TObjectPtr<UPawnMovementComponent> PawnMovement = PawnOwner->GetMovementComponent();
		if (IsValid(PawnOwner->GetMovementComponent()))
		{
			return PawnMovement->GetLastInputVector();
		}
	}

	return FVector::ZeroVector;
}

APawn* UTD_InputManagerComponent::GetPawn() const
{
	TObjectPtr<APawn> Pawn = nullptr;
	
	if (GetOwner()->IsA(APawn::StaticClass()))
	{
		Pawn = Cast<APawn>(GetOwner());
	}
	else if (GetOwner()->IsA(AController::StaticClass()))
	{
		const AController* Controller = Cast<AController>(GetOwner());
		Pawn = Controller->GetPawn();
	}
	else if (GetOwner()->IsA(APlayerState::StaticClass()))
	{
		const APlayerState* PlayerState = Cast<APlayerState>(GetOwner());
		Pawn = PlayerState->GetPawn();
	}

	ensureAlwaysMsgf(IsValid(Pawn), TEXT("Unable to retrieve the owning Pawn."));
	return Pawn;
}

AController* UTD_InputManagerComponent::GetController() const
{
	TObjectPtr<AController> Controller = nullptr;

	if (GetOwner()->IsA(APawn::StaticClass()))
	{
		const TObjectPtr<const APawn> Pawn = Cast<APawn>(GetOwner());
		Controller = Pawn->GetController();
	}
	else if (GetOwner()->IsA(AController::StaticClass()))
	{
		Controller = Cast<AController>(GetOwner());
	}
	else if (GetOwner()->IsA(APlayerState::StaticClass()))
	{
		const TObjectPtr<APlayerState> PlayerState = Cast<APlayerState>(GetOwner());
		Controller = PlayerState->GetOwningController();
	}

	ensureAlwaysMsgf(IsValid(Controller), TEXT("Unable to retrieve the owning Controller."));
	return Controller;
}

bool UTD_InputManagerComponent::IsLocallyControlled() const
{
	const TObjectPtr<const APawn> MyPawn = GetPawn();
	return IsValid(MyPawn) && MyPawn->IsLocallyControlled();
}

UEnhancedInputLocalPlayerSubsystem* UTD_InputManagerComponent::GetEnhancedInputSubsystem(AController* Controller) const
{
	if (IsValid(Controller))
	{
		const APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (IsValid(PlayerController))
		{
			const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
			if (IsValid(LocalPlayer))
			{
				return LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			}
		}
	}
	return nullptr;
}

bool UTD_InputManagerComponent::HasSetupData(const UTD_InputSetupDataAsset* SetupData) const
{
	for (auto It(ProcessedSetups.CreateConstIterator()); It; ++It)
	{
		if (It.Value().SourceData == SetupData)
		{
			return true;
		}
	}
	return false;
}

bool UTD_InputManagerComponent::HasInputMappingContext(const UInputMappingContext* InputMappingContext) const
{
	check(IsValid(InputMappingContext));
	
	const UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem(OwnerController.Get());
	return IsValid(Subsystem) && Subsystem->HasMappingContext(InputMappingContext);
}

bool UTD_InputManagerComponent::HasCompatibleInputHandler(const UInputAction* InputAction,
	const ETriggerEvent& TriggerEvent) const
{
	for (auto It(ProcessedSetups.CreateConstIterator()); It; ++It)
	{
		if (It.Value().SourceData->HasCompatibleInputHandler(InputAction, TriggerEvent))
		{
			return true;
		}
	}

	return false;
}

void UTD_InputManagerComponent::AddInputSetupData(const UTD_InputSetupDataAsset* SetupData)
{
	check(SetupData);

	// 确保尚未处理此设置。
	if (HasSetupData(SetupData))
	{
		UE_LOG(TD_Log_InputComponent, Warning, TEXT("[%s] Received a duplicated Input Setup (%s)!"),
			 *GetNameSafe(GetOwner()), *GetNameSafe(SetupData));
        
		return;
	}
    
	// 还要确保我们没有来自其他设置的相同 IMC。
	const TObjectPtr<UInputMappingContext> NewContext = SetupData->InputMappingContext;
	if (HasInputMappingContext(NewContext))
	{
		const FTD_ProcessedInputSetup OriginalSetup = *ProcessedSetups.Find(NewContext);
		UE_LOG(TD_Log_InputComponent, Warning, TEXT("[%s] Detected a duplicated IMC (%s)! Original Setup: '%s', Provided Setup: '%s'"),
			 *GetNameSafe(GetOwner()), *GetNameSafe(NewContext), *GetNameSafe(OriginalSetup.SourceData), *GetNameSafe(SetupData));

		return;
	}
    
	TArray<FTD_ProcessedBinding> Bindings;
	AddInputMappingContext(NewContext, SetupData->Priority, Bindings);

	if (!Bindings.IsEmpty())
	{
		const FTD_ProcessedInputSetup Setup(SetupData, Bindings);
		ProcessedSetups.Add(NewContext, Setup);

		UE_LOG(TD_Log_InputComponent, Log, TEXT("[%s] Added Setup Data %s with %d bindings."),
			*GetNameSafe(GetOwner()), *GetNameSafe(SetupData), Bindings.Num());
	}
	else
	{
		// 我们没有任何绑定，因此这个 Context 毫无意义。将其删除，以便稍后可以再次添加。
		RemoveInputMappingContext(NewContext);

		UE_LOG(TD_Log_InputComponent, Warning, TEXT("[%s] Discarded Setup Data %s as it has no bindings."),
			*GetNameSafe(GetOwner()), *GetNameSafe(SetupData));
	}
}

void UTD_InputManagerComponent::RemoveInputSetupData(const UTD_InputSetupDataAsset* SetupData)
{
	if (ensure(IsValid(SetupData)) && HasSetupData(SetupData))
	{
		// Make sure the buffered handlers from this setup won't be executed later.
		// for (TObjectPtr<UTD_InputHandle> Handler : SetupData->InputHandlers)
		// {
		// 	BufferedCommands.RemoveAll([Handler](const FTD_BufferedInputCommand& Command)
		// 		{ return Command.Handler == Handler; });
		// }
        
		RemoveInputMappingContext(SetupData->InputMappingContext);
	}
}

int32 UTD_InputManagerComponent::SendGameplayEventToOwner(const FGameplayTag& GameplayEventTag, const FInputActionValue& Value,
	const UInputAction* InputAction, bool bSendLocally, bool bSendToServer) const
{
	int32 Activations = 0;
	const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController());
    
	if (IsValid(PlayerController) && ensureMsgf(GameplayEventTag.IsValid(), TEXT("The Gameplay Event Tag must be valid.")))
	{
		if ((bSendLocally && PlayerController->IsLocalController()) || (bSendToServer && GetOwner()->HasAuthority()))
		{
			// Local execution for either local client or authoritative version.
			Activations = FTD_InputHandlerHelpers::SendGameplayEvent(this, GameplayEventTag, Value, InputAction);
		}
        
		if (PlayerController->IsLocalController() && bSendToServer && !GetOwner()->HasAuthority())
		{
			// On local client and we need to send this event to the server.
			Server_SendGameplayEventToOwner(GameplayEventTag, Value, InputAction);
		}

		if (GetOwner()->HasAuthority() && bSendLocally && !PlayerController->IsLocalController())
		{
			// On a server and we need to send this event to the client.
			Client_SendGameplayEventToOwner(GameplayEventTag, Value, InputAction);
		}
	}

	return Activations;
}

void UTD_InputManagerComponent::SetupInputComponent(const APawn* Pawn)
{
	check(Pawn);
	
	InputComponent = Cast<UEnhancedInputComponent>(Pawn->InputComponent);
	if (ensureMsgf(InputComponent, TEXT("Please make sure to use the EnhancedInputComponent!")))
	{
		if (IsValid(OwnerController))
		{
			for (const TObjectPtr<const UTD_InputSetupDataAsset> SetupData : InputHandlerSetup)
			{
				AddInputSetupData(SetupData);
			}
		}
	}
}

void UTD_InputManagerComponent::AddInputMappingContext(const UInputMappingContext* InputMappingContext, int32 Priority,
	TArray<FTD_ProcessedBinding>& OutBindings)
{
	OutBindings.Reset();
    
	if (IsValid(InputComponent) && ensure(IsValid(InputMappingContext)))
	{
	    if (IsValid(InputMappingContext) && !HasInputMappingContext(InputMappingContext))
	    {
	        const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = GetEnhancedInputSubsystem(OwnerController.Get());
	        check(IsValid(Subsystem));
	        
	        Subsystem->AddMappingContext(InputMappingContext, Priority);
	
	        TArray<TObjectPtr<const UInputAction>> ActionsToBind;
	        ActionsToBind.Reserve(InputMappingContext->GetMappings().Num());
	        OutBindings.Reserve(ActionsToBind.Num());

	        // Ensure that we only process each action once, regardless of how many keys are assigned to them.
	        for (const auto& KeyMapping: InputMappingContext->GetMappings())
	        {
	            ActionsToBind.AddUnique(KeyMapping.Action);
	        }

	        // Create bindings for each unique action, for every tracked trigger and then store the Handle.
	        for (const auto InputAction : ActionsToBind)
	        {
	            const TArray<ETriggerEvent> TrackedEvents = GetDefault<UTD_InputSetting>()->TrackedEvents;
	            for (const ETriggerEvent TriggerEvent : TrackedEvents)
	            {
	                FEnhancedInputActionHandlerInstanceSignature::TMethodPtr<ThisClass> DispatchFunction = nullptr;
	                switch (TriggerEvent)
	                {
	                case ETriggerEvent::Started:
	                    DispatchFunction = &ThisClass::DispatchStartedEvent;
	                    break;
                    case ETriggerEvent::Triggered:
                        DispatchFunction = &ThisClass::DispatchTriggeredEvent;
	                    break;
                    case ETriggerEvent::Ongoing:
	                    DispatchFunction = &ThisClass::DispatchOngoingEvent;
	                    break;
	                case ETriggerEvent::Completed:
	                    DispatchFunction = &ThisClass::DispatchCompletedEvent;
	                    break;
                    case ETriggerEvent::Canceled:
	                    DispatchFunction = &ThisClass::DispatchCancelledEvent;
	                    break;
                    default:
                        const UEnum* EnumPtr = FindObject<UEnum>(GetOuter(), TEXT("ETriggerEvent"), true);
	                    UE_LOG(TD_Log_InputComponent, Warning, TEXT("[%s] Unable to handle Trigger Event '%s'."),
                            *GetNameSafe(GetOwner()), *EnumPtr->GetValueAsString(TriggerEvent));
                    }

	                if (DispatchFunction != nullptr)
	                {
	                    FEnhancedInputActionEventBinding* Handle = &InputComponent->BindAction(InputAction, TriggerEvent, this, DispatchFunction);
	                    if (Handle != nullptr)
	                    {
	                        FTD_ProcessedBinding Binding(InputAction, TriggerEvent, Handle);
	                        OutBindings.Add(Binding);
	                    }
	                }
	            }
	        }
	    }
	}
}

void UTD_InputManagerComponent::DispatchStartedEvent(const FInputActionInstance& ActionInstance)
{
}

void UTD_InputManagerComponent::DispatchTriggeredEvent(const FInputActionInstance& ActionInstance)
{
}

void UTD_InputManagerComponent::DispatchOngoingEvent(const FInputActionInstance& ActionInstance)
{
}

void UTD_InputManagerComponent::DispatchCompletedEvent(const FInputActionInstance& ActionInstance)
{
}

void UTD_InputManagerComponent::DispatchCancelledEvent(const FInputActionInstance& ActionInstance)
{
}

void UTD_InputManagerComponent::Dispatch(const FInputActionInstance& ActionInstance, ETriggerEvent ActualTrigger)
{
	const FInputActionValue Value = ActionInstance.GetValue();
	const TObjectPtr<const UInputAction> InputAction = ActionInstance.GetSourceAction();
	//
	// TArray<FBufferedInputCommand> CandidateCommands;
	// const TObjectPtr<UActorComponent> InputBuffer = GetInputBufferComponent();
	// const bool bIsUsingBuffer = IsValid(InputBuffer) && Execute_IsInputBufferOpen(InputBuffer);
	//
	// for (auto It(ProcessedSetups.CreateConstIterator()); It; ++It)
	// {
	// 	for (TObjectPtr<UNinjaInputHandler> Handler : ProcessedSetups.Find(It.Key())->SourceData->InputHandlers)
	// 	{
	// 		if (IsValid(Handler) && Handler->CanHandle(ActualTrigger, InputAction))
	// 		{
	// 			if (bIsUsingBuffer && Handler->CanBeBuffered())
	// 			{
	// 				UE_LOG(LogNinjaInputManagerComponent, VeryVerbose, TEXT("[%s] Input Action %s will be saved as an Input Buffer candidate."),
	// 					*GetNameSafe(GetOwner()), *GetNameSafe(InputAction));
 //                
	// 				FBufferedInputCommand NewCommand(this, InputAction, Handler, Value, ActualTrigger);
	// 				CandidateCommands.AddUnique(NewCommand);
	// 			}
	// 			else
	// 			{
	// 				UE_LOG(LogNinjaInputManagerComponent, VeryVerbose, TEXT("[%s] Input Action %s will trigger handler %s."),
	// 					*GetNameSafe(GetOwner()), *GetNameSafe(InputAction), *GetNameSafe(Handler));
	//
	// 				Handler->SetWorld(GetWorld());
	// 				Handler->HandleInput(this, Value, ActualTrigger, InputAction);
	// 			}
	// 		}
	// 	}
	// }
	//
	// Execute_BufferInputCommands(InputBuffer, CandidateCommands);
}

void UTD_InputManagerComponent::RemoveInputMappingContext(const UInputMappingContext* InputMappingContext)
{
}

void UTD_InputManagerComponent::ClearInputSetup()
{
}

void UTD_InputManagerComponent::OnPawnRestarted(APawn* Pawn)
{
}

void UTD_InputManagerComponent::OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
}

void UTD_InputManagerComponent::Client_SendGameplayEventToOwner_Implementation(const FGameplayTag& GameplayEventTag,
	const FInputActionValue& Value, const UInputAction* InputAction) const
{
}

void UTD_InputManagerComponent::Server_SendGameplayEventToOwner_Implementation(const FGameplayTag& GameplayEventTag,
                                                                               const FInputActionValue& Value, const UInputAction* InputAction) const
{
}

void UTD_InputManagerComponent::GetVectorForAxis_Implementation(const EAxis::Type Axis, FVector& OutReference) const
{
	if (IsValid(ForwardReference))
	{
		const FRotator ReferenceYawRotation = ForwardReference->GetComponentRotation();
		OutReference = FRotationMatrix(ReferenceYawRotation).GetScaledAxis(Axis);
	}
	else if (IsValid(OwnerController))
	{
		FRotator ControlRotation = OwnerController->GetControlRotation();
		ControlRotation.Roll = 0.f;
		ControlRotation.Pitch = 0.f;
		OutReference = FRotationMatrix(ControlRotation).GetScaledAxis(Axis);
	}
	else switch (Axis)
	{
	case EAxis::X:
		OutReference = GetOwner()->GetActorForwardVector();
		break;
	case EAxis::Y:
		OutReference = GetOwner()->GetActorRightVector();
		break;
	case EAxis::Z:
		OutReference = GetOwner()->GetActorUpVector();
		break;
	default:
		checkNoEntry();
	}
}
