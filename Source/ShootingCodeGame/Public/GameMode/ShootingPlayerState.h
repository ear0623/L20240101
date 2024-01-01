// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShootingPlayerState.generated.h"

// 괄호안의 작성 양식 - 변수형, 변수이름, 첫번째 변수 타입과 이름, 두번째 변수 타입과 이름
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
	// RepNotify 함수 - 해당 함수는 반드시 UFUNCTION() 이여야 합니다.
	UFUNCTION()
	void OnRep_CurHp();

public:

	// RepNotify 변수
	UPROPERTY(ReplicatedUsing = OnRep_CurHp)
	float m_CurHp;

	// Delegate 변수
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateHp m_Dele_UpdateHp;
};
