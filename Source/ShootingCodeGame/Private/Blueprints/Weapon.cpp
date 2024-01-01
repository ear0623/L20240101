// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprints/Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	WeaponMesh->SetCollisionProfileName("Weapon");
	WeaponMesh->SetSimulatePhysics(true);
	SetRootComponent(WeaponMesh);

	bReplicates = true;
	SetReplicateMovement(true);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 아래의 함수를 사용해서 서버에서 동기화를 시켜줍니다.
	DOREPLIFETIME(AWeapon, m_pOwnChar);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::EventTrigger_Implementation()
{
	m_pOwnChar->PlayAnimMontage(m_ShootMontage);
}

void AWeapon::EventShoot_Implementation()
{
	// 총기 이펙트 추가 코드
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_FireEffect, 
		WeaponMesh->GetSocketLocation("muzzle"),
		WeaponMesh->GetSocketRotation("muzzle"),
		FVector(0.1f, 0.1f, 0.1f));

	// 총기 사운드 추가 코드
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), m_SoundBase,
		WeaponMesh->GetSocketLocation("muzzle"));

	// 플레이어 카메라를 가져오기 위해서 컨트롤러를 가져옵니다.
	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();

	// 이제 위에서 가져온 플레이어 변수로부터 카메라 위치와 방향을 가져옵니다.
	FVector CameraLoc = pPlayer0->PlayerCameraManager->GetCameraLocation();
	FVector CameraForward = pPlayer0->PlayerCameraManager->GetActorForwardVector();

	// 카메라 시작점 부터의 거리를 카메라 위치로부터 구하는 방법입니다.
	FVector vStart = (CameraForward * GetFireStartLenghth()) + CameraLoc;

	// 가져온 위치와 방향으로부터 얼마나 멀리 나아가는지 거리값
	FVector vEnd = CameraForward * 1000 + CameraLoc;

	ReqShoot(vStart, vEnd);
}

void AWeapon::EventReload_Implementation()
{
	m_pOwnChar->PlayAnimMontage(m_ReloadMontage);
}

void AWeapon::EventPickUp_Implementation(ACharacter* pOwnChar)
{
	m_pOwnChar = pOwnChar;

	WeaponMesh->SetSimulatePhysics(false);
	AttachToComponent(pOwnChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));
}

void AWeapon::EventDrop_Implementation(ACharacter* pOwnChar)
{
	m_pOwnChar = nullptr;

	WeaponMesh->SetSimulatePhysics(true);
	AttachToComponent(pOwnChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));
}

void AWeapon::ReqShoot_Implementation(FVector vStart, FVector vEnd)
{
	// Hit 된 물체를 담기 위한 변수
	FHitResult result;

	// 콜리전 타입을 담을 변수를 만들고 그 안에 타입들을 추가합니다.
	FCollisionObjectQueryParams collisionObjParams;
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

	// 자기 자신은 총에 피격되면 안되기 때문에 아래처럼 추가해줍니다.
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(m_pOwnChar);

	// LineTrace - Hit 된 객체, 시작 위치, 종료 위치, 히트시 처리할 콜리전 종류, 히트를 무시할 종류
	bool isHit = GetWorld()->LineTraceSingleByObjectType(result, vStart, vEnd, collisionObjParams, collisionParams);

	// LineTrace 시각적 표현 - 현재 레벨, 시작 위치, 종료 위치, 색상, 유지 시간
	DrawDebugLine(GetWorld(), vStart, vEnd, FColor::Red, false, 5.0f);

	// Hit 된 물체 검증
	if (false == isHit)
		return;

	// Hit 된 물체를 가져오기 위해서 형변환 코드를 작성해줍니다.
	ACharacter* pHitChar = Cast<ACharacter>(result.GetActor());
	if (false == IsValid(pHitChar))
		return;

	// Hit 된 물체가 있다면 위의 코드를 모두 타고 내려와서 출력문을 통해서 확인을 해줍니다.
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Yellow, FString::Printf(TEXT("HitChar = %s"), *pHitChar->GetName()));

	// Hit 된 캐릭터, 데미지 수치, 데미지를 준 캐릭터, 데미지를 준 주체, 데미지 종류(수류탄, 총 등..) 지금은 기본값으로
	UGameplayStatics::ApplyDamage(pHitChar, 10.0f, m_pOwnChar->GetController(), this, UDamageType::StaticClass());
}

float AWeapon::GetFireStartLenghth()
{
	if (false == IsValid(m_pOwnChar))
		return 0.0f;

	USpringArmComponent* pArm = Cast<USpringArmComponent>(m_pOwnChar->GetComponentByClass(USpringArmComponent::StaticClass()));
	if (false == IsValid(pArm))
		return 0.0f;

	return pArm->TargetArmLength + 100;
}
