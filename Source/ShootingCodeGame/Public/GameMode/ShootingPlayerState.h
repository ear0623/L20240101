// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShootingPlayerState.generated.h"

// ��ȣ���� �ۼ� ��� - ������, �����̸�, ù��° ���� Ÿ�԰� �̸�, �ι�° ���� Ÿ�԰� �̸�
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHp, float, CurHp, float, MaxHp);


UCLASS()
class SHOOTINGCODEGAME_API AShootingPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AShootingPlayerState();

// ================= Network Function Section
public:
	UFUNCTION()
	void AddDamage(float Damage);
	
// ================= Replicate Function Section
public:
	// RepNotify �Լ� - �ش� �Լ��� �ݵ�� UFUNCTION() �̿��� �մϴ�.
	UFUNCTION()
	void OnRep_CurHp();

public:

	// RepNotify ����
	UPROPERTY(ReplicatedUsing = OnRep_CurHp)
	float m_CurHp;

	// Delegate ����
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateHp m_Dele_UpdateHp;
};
