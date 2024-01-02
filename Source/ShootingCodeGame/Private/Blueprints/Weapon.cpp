// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprints/Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameMode/ShootingHUD.h"


// Sets default values
AWeapon::AWeapon():m_Ammo(30)
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

	// �Ʒ��� �Լ��� ����ؼ� �������� ����ȭ�� �����ݴϴ�.
	DOREPLIFETIME(AWeapon, m_Ammo);
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
	if (false == bIsCanShoot())
		return;

	// �ѱ� ����Ʈ �߰� �ڵ�
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_FireEffect, 
		WeaponMesh->GetSocketLocation("muzzle"),
		WeaponMesh->GetSocketRotation("muzzle"),
		FVector(0.1f, 0.1f, 0.1f));

	// �ѱ� ���� �߰� �ڵ�
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), m_SoundBase,
		WeaponMesh->GetSocketLocation("muzzle"));

	// �÷��̾� ī�޶� �������� ���ؼ� ��Ʈ�ѷ��� �����ɴϴ�.
	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();
	if (pPlayer0 != m_pOwnChar->GetController())
		return;

	// ���� ������ ������ �÷��̾� �����κ��� ī�޶� ��ġ�� ������ �����ɴϴ�.
	FVector CameraLoc = pPlayer0->PlayerCameraManager->GetCameraLocation();
	FVector CameraForward = pPlayer0->PlayerCameraManager->GetActorForwardVector();

	// ī�޶� ������ ������ �Ÿ��� ī�޶� ��ġ�κ��� ���ϴ� ����Դϴ�.
	FVector vStart = (CameraForward * GetFireStartLenghth()) + CameraLoc;

	// ������ ��ġ�� �������κ��� �󸶳� �ָ� ���ư����� �Ÿ���
	FVector vEnd = CameraForward * 1000 + CameraLoc;

	ReqShoot(vStart, vEnd);
}

void AWeapon::EventReload_Implementation()
{
	
	m_pOwnChar->PlayAnimMontage(m_ReloadMontage);
	//FTimerManager& timerManager = GetWorld()->GetTimerManager(); 
	//timerManager.SetTimer(th_BindWeapon,this,&AWeapon::ReloadCall,2.17f);
}

void AWeapon::EventResetAmmo_Implementation()
{
	SetAmmo(30);
}

void AWeapon::EventPickUp_Implementation(ACharacter* pOwnChar)
{
	m_pOwnChar = pOwnChar;
	m_pOwnChar->bUseControllerRotationYaw = true;
	WeaponMesh->SetSimulatePhysics(false);
	AttachToComponent(pOwnChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));
	OnUpdateAmmoToHud(m_Ammo);
}

void AWeapon::EventDrop_Implementation(ACharacter* pOwnChar)
{
	OnUpdateAmmoToHud(0);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	WeaponMesh->SetSimulatePhysics(true);
	m_pOwnChar->bUseControllerRotationYaw = false;
	m_pOwnChar = nullptr;
	
}

void AWeapon::ReqShoot_Implementation(FVector vStart, FVector vEnd)
{
	if (false == UseAmmo())
		return;
	// Hit �� ��ü�� ��� ���� ����
	FHitResult result;

	// �ݸ��� Ÿ���� ���� ������ ����� �� �ȿ� Ÿ�Ե��� �߰��մϴ�.
	FCollisionObjectQueryParams collisionObjParams;
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

	// �ڱ� �ڽ��� �ѿ� �ǰݵǸ� �ȵǱ� ������ �Ʒ�ó�� �߰����ݴϴ�.
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(m_pOwnChar);

	// LineTrace - Hit �� ��ü, ���� ��ġ, ���� ��ġ, ��Ʈ�� ó���� �ݸ��� ����, ��Ʈ�� ������ ����
	bool isHit = GetWorld()->LineTraceSingleByObjectType(result, vStart, vEnd, collisionObjParams, collisionParams);

	// LineTrace �ð��� ǥ�� - ���� ����, ���� ��ġ, ���� ��ġ, ����, ���� �ð�
	DrawDebugLine(GetWorld(), vStart, vEnd, FColor::Red, false, 5.0f);

	// Hit �� ��ü ����
	if (false == isHit)
		return;

	// Hit �� ��ü�� �������� ���ؼ� ����ȯ �ڵ带 �ۼ����ݴϴ�.
	ACharacter* pHitChar = Cast<ACharacter>(result.GetActor());
	if (false == IsValid(pHitChar))
		return;

	// Hit �� ��ü�� �ִٸ� ���� �ڵ带 ��� Ÿ�� �����ͼ� ��¹��� ���ؼ� Ȯ���� ���ݴϴ�.
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Yellow, FString::Printf(TEXT("HitChar = %s"), *pHitChar->GetName()));

	// Hit �� ĳ����, ������ ��ġ, �������� �� ĳ����, �������� �� ��ü, ������ ����(����ź, �� ��..) ������ �⺻������
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

bool AWeapon::bIsCanShoot()
{
	if (m_Ammo <= 0)
		return false;

	return true;
}

bool AWeapon::UseAmmo()
{
	if (false == bIsCanShoot())
		return false;
	m_Ammo =m_Ammo - 1;

	m_Ammo = FMath::Clamp(m_Ammo, 0, 30);

	OnRep_Ammo();
	return true;
}


void AWeapon::SetAmmo(int Ammo)
{
	m_Ammo = Ammo;
	OnRep_Ammo();
}

void AWeapon::OnUpdateAmmoToHud(int Ammo)
{
	if (nullptr == m_pOwnChar)
		return;

	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();
	if (m_pOwnChar->GetController() != pPlayer0)
		return;

	AShootingHUD* pHUD = Cast<AShootingHUD>(pPlayer0->GetHUD());
	if (nullptr == pHUD)
		return;

	pHUD->OnUpdateMyAmmo(m_Ammo);
}

void AWeapon::OnRep_Ammo()
{
	OnUpdateAmmoToHud(m_Ammo);
}
