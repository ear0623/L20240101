// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

// �������Ʈ�� �� �������̽��� ������ �� �ֵ��� �Ϸ���, Blueprintable ��Ÿ������ �����ڸ� ����ؾ� �մϴ�. 
// �������Ʈ Ŭ������ ������� ��� �������̽� �Լ��� BlueprintNativeEvent �Ǵ� BlueprintImplementableEvent ���� �մϴ�.
// BlueprintCallable ��ŷ�� �Լ��� ������ ȣ���� ������ ���̳�, ������ �Ұ����մϴ�. 
// �ٸ� ��� �Լ��� �������Ʈ���� ������ �� ���� ���Դϴ�.

// UWeaponInterface �� UInterface Ŭ������ ��ӹ޾� ������ �𸮾��� �������̽��Դϴ�.
UINTERFACE(Blueprintable)
class UWeaponInterface : public UInterface
{
	// GENERATED_BODY() �� �𸮾� ���� �ڵ� ���� �� �ʿ��� �߰� �ڵ带 �����ϱ� ���� ��ũ���Դϴ�.
	GENERATED_BODY()
};

/**
 * SHOOTINGCODEGAME_API �� Ŭ������ ��������(dll�� ������� ��⿡�� ��� �����ϵ��� �ϴ�) ��ũ���Դϴ�. 
 * �̴� �ش� �������̽��� ��� �ܺο����� ���� �� �ֵ��� �ϴ� ������ �մϴ�.
 */
class SHOOTINGCODEGAME_API IWeaponInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventTrigger();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventShoot();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventReload();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventResetAmmo();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventPickUp(ACharacter* pOwnChar);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventDrop(ACharacter* pOwnChar);
};
