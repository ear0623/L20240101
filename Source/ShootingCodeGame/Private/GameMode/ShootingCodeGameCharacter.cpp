// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/ShootingCodeGameCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"
#include "GameMode/ShootingPlayerState.h"
#include "Blueprints/Weapon.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AShootingCodeGameCharacter

AShootingCodeGameCharacter::AShootingCodeGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AShootingCodeGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// (Replicate 관련) 아래의 함수를 사용해서 서버에서 동기화를 시켜줍니다.
	DOREPLIFETIME(AShootingCodeGameCharacter, ControlRot);

	// 시작부터 무기를 캐릭터에 붙여줍니다.
	//DOREPLIFETIME(AShootingCodeGameCharacter, m_EquipWeapon);
}

void AShootingCodeGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AShootingCodeGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// 서버에서만 정보를 넣어줍니다.
	if (HasAuthority() == true)
	{
		ControlRot = GetControlRotation();
	}

}

float AShootingCodeGameCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Yellow, 
		FString::Printf(TEXT("TakeDamage DamageAmount = %f, EventInstigator = %s, DamageCasuse = %s"), 
		DamageAmount, 
		*EventInstigator->GetName(), 
		*DamageCauser->GetName()));

	AShootingPlayerState* ps = Cast<AShootingPlayerState>(GetPlayerState());
	if (false == IsValid(ps))
	{
		GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Red, TEXT("PS is not valid !!"));
		return 0.0f;
	}

	ps->AddDamage(DamageAmount);
	
	return DamageAmount;
}


//////////////////////////////////////////////////////////////////////////
// NetWork

// ============================================= [ TakeWeapon ]
// 1. (F 키 입력 이벤트 발생)무기 획득 함수 입력키를 바탕으로 호출
void AShootingCodeGameCharacter::TakeWeapon(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Yellow, TEXT("Junsik Babo TakeWeapon"));
	RequestTakeWeapon();
}

// 2. 무기 획득 함수 Server 로 관련 코드들을 실행 및 Response 함수 호출
void AShootingCodeGameCharacter::RequestTakeWeapon_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::White, TEXT("Junsik Babo Req RequestTakeWeapon"));
	AActor* pNearestActor = FindNearestWeapon();

	if (false == IsValid(pNearestActor))
		return;

	pNearestActor->SetOwner(GetController());

	ResponseTakeWeapon(pNearestActor);
}

// 3. Multicast 기반 함수를 통해 클라이언트와 서버 모두에게 전송
void AShootingCodeGameCharacter::ResponseTakeWeapon_Implementation(AActor* PickActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::White, TEXT("Junsik Babo Res ResponseTakeWeapon"));
	
	m_EquipWeapon = PickActor;

	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);

	if (nullptr == InterfaceObj)
		return;

	InterfaceObj->Execute_EventPickUp(m_EquipWeapon, this);
}

// 4. 클라이언트로부터 호출될 함수
void AShootingCodeGameCharacter::ResponseTakeWeaponClient_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::White, TEXT("Junsik Babo Res ResponseTakeWeaponClient"));
}
// ===========================================================


// ============================================= [ DropWeapon ]
void AShootingCodeGameCharacter::DropWeapon(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Yellow, TEXT("Junsik Babo DropWeapon"));
	RequestDropWeapon();
}

void AShootingCodeGameCharacter::RequestDropWeapon_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::White, TEXT("Junsik Babo Request DropWeapon"));
	AActor* pNearestActor = FindNearestWeapon();

	if (false == IsValid(pNearestActor))
		return;

	ResponseDropWeapon(pNearestActor);
}

void AShootingCodeGameCharacter::ResponseDropWeapon_Implementation(AActor* PickActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::White, TEXT("Junsik Babo Response DropWeapon"));

	m_EquipWeapon = PickActor;

	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);

	if (nullptr == InterfaceObj)
		return;

	InterfaceObj->Execute_EventDrop(m_EquipWeapon, this);
}

void AShootingCodeGameCharacter::ResponseDropWeaponClient_Implementation()
{

}
// ===========================================================


// ============================================= [ Reload ]
void AShootingCodeGameCharacter::Reload(const FInputActionValue& Value)
{
	RequestReload();
}

void AShootingCodeGameCharacter::RequestReload_Implementation()
{
	ResponseReload();
}

void AShootingCodeGameCharacter::ResponseReload_Implementation()
{
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);

	if (nullptr == InterfaceObj)
		return;

	InterfaceObj->Execute_EventReload(m_EquipWeapon);

	UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
}
// ===========================================================


// ============================================= [ Trigger ]
void AShootingCodeGameCharacter::Trigger(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Yellow, TEXT("Junsik Babo Shoot"));
	RequestTrigger();
}

void AShootingCodeGameCharacter::RequestTrigger_Implementation()
{
	ResponseTrigger();
}

void AShootingCodeGameCharacter::ResponseTrigger_Implementation()
{
	// 인터페이스로 캐스팅 하는 방법(C++ 에서의 상속)
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);

	if (nullptr == InterfaceObj)
		return;

	// 인터페이스 호출 - 인자값(객체), 파라미터(우리는 사용하지 않아서 없음)
	InterfaceObj->Execute_EventTrigger(m_EquipWeapon);
}
// ===========================================================


//////////////////////////////////////////////////////////////////////////
// Input Action Binding

void AShootingCodeGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShootingCodeGameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShootingCodeGameCharacter::Look);

		// Trigger
		EnhancedInputComponent->BindAction(TriggerAction, ETriggerEvent::Started, this, &AShootingCodeGameCharacter::Trigger);

		// TakeWeapon
		EnhancedInputComponent->BindAction(TakeWeaponAction, ETriggerEvent::Started, this, &AShootingCodeGameCharacter::TakeWeapon);

		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AShootingCodeGameCharacter::Reload);

		// DropWeapon
		EnhancedInputComponent->BindAction(DropWeaponAction, ETriggerEvent::Started, this, &AShootingCodeGameCharacter::DropWeapon);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


//////////////////////////////////////////////////////////////////////////
// Custom Function

void AShootingCodeGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}


void AShootingCodeGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void AShootingCodeGameCharacter::EquipTestWeapon(TSubclassOf<class AWeapon> WeaponClass)
{
	// 서버에서만 무기를 스폰 시키기 위해서 해줍니다.
	if (false == HasAuthority())
		return;

	// 무기 스폰 및 좌표 초기값 지정
	m_EquipWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector(0, 0, 0), FRotator(0, 0, 0));

	// 형변환 통해서 가져오기 실패 시 아무것도 하지 않음(return)
	AWeapon* pWeapon = Cast<AWeapon>(m_EquipWeapon);
	if (false == IsValid(pWeapon))
		return;

	pWeapon->m_pOwnChar = this;

	// 해당 컨트롤러에 Owner 즉, 소유권을 부여해줍니다.
	TestWeaponSetOwner();

	// 임시로 Weapon 붙이기 - SnapToTarget 즉 무기를 붙이되 스케일은 미포함, 소켓 이름은 Weapon
	m_EquipWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Weapon"));
}


void AShootingCodeGameCharacter::TestWeaponSetOwner()
{
	if (IsValid(GetController()))
	{
		m_EquipWeapon->SetOwner(GetController());
		return;
	}

	FTimerManager& tm = GetWorld()->GetTimerManager();
	tm.SetTimer(th_BindSetOwner, this, &AShootingCodeGameCharacter::TestWeaponSetOwner, 0.1f, false);
}


AActor* AShootingCodeGameCharacter::FindNearestWeapon()
{
	// 주변의 무기를 모두 가져와야 하기 때문에 배열로 변수 선언
	TArray<AActor*> actors;
	// 액터 리스트, 검색할 클래스 명
	GetCapsuleComponent()->GetOverlappingActors(actors, AWeapon::StaticClass());

	// 기본 거리값이 0 이면 탐색이 의미가 없기 때문에 탐색 범위 크기를 아래처럼 크게 해줍니다.
	double nearestDist = 9999999.0f;
	// 검출된 무기를 담을 변수입니다.
	AActor* pNearestActor = nullptr;
	for (AActor* pTarget : actors)
	{
		// 거리 구하는 기능을 이용합니다.(FVector::Distance)
		// 오버랩 된 액터로부터 무기 까지의 거리 구하는 함수
		double dist = FVector::Distance(GetActorLocation(), pTarget->GetActorLocation());
		// 조건문을 통해서 거리값이 nearestDist 보다 작으면 가져오게 해줍니다.
		if (dist >= nearestDist)
			continue;

		nearestDist = dist;
		pNearestActor = pTarget;
	}

	return pNearestActor;
}