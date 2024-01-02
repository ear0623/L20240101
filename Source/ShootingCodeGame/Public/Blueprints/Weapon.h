// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"



UCLASS()
class SHOOTINGCODEGAME_API AWeapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventTrigger();

	virtual void EventTrigger_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventShoot();

	virtual void EventShoot_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventReload();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventResetAmmo();

	virtual void EventResetAmmo_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventPickUp(ACharacter* pOwnChar);

	virtual void EventPickUp_Implementation(ACharacter* pOwnChar) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventDrop(ACharacter* pOwnChar);

	virtual void EventDrop_Implementation(ACharacter* pOwnChar) override;

public:
	// 서버로부터 호출될 사격용 이벤트 함수(RPC 함수)
	UFUNCTION(Server, Reliable)
	void ReqShoot(FVector vStart, FVector vEnd);

public:
	float GetFireStartLenghth();

	UFUNCTION(BlueprintPure)
	bool bIsCanShoot();

	bool UseAmmo();

	void SetAmmo(int Ammo);

	UFUNCTION(BlueprintCallable)
	void OnUpdateAmmoToHud(int Ammo);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* WeaponMesh;

	// 캐릭터 저장을 위한 변수
	UPROPERTY(BlueprintReadWrite)
	ACharacter* m_pOwnChar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_ReloadMontage;

	// 총기 발사 이펙트 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* m_FireEffect;

	// 총기 발사 사운드 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* m_SoundBase;

	UFUNCTION()
	void OnRep_Ammo();

	UPROPERTY(ReplicatedUsing = OnRep_Ammo)
	int m_Ammo;

	FTimerHandle th_BindWeapon;
};
