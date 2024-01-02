// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

// 블루프린트가 이 인터페이스를 구현할 수 있도록 하려면, Blueprintable 메타데이터 지정자를 사용해야 합니다. 
// 블루프린트 클래스가 덮어쓰려는 모든 인터페이스 함수는 BlueprintNativeEvent 또는 BlueprintImplementableEvent 여야 합니다.
// BlueprintCallable 마킹된 함수는 여전히 호출은 가능할 것이나, 덮어쓰기는 불가능합니다. 
// 다른 모든 함수는 블루프린트에서 접근할 수 없을 것입니다.

// UWeaponInterface 는 UInterface 클래스를 상속받아 구현한 언리얼의 인터페이스입니다.
UINTERFACE(Blueprintable)
class UWeaponInterface : public UInterface
{
	// GENERATED_BODY() 는 언리얼 엔진 코드 생성 시 필요한 추가 코드를 생성하기 위한 매크로입니다.
	GENERATED_BODY()
};

/**
 * SHOOTINGCODEGAME_API 는 클래스를 내보내는(dll로 만들어진 모듈에서 사용 가능하도록 하는) 매크로입니다. 
 * 이는 해당 인터페이스가 모듈 외부에서도 사용될 수 있도록 하는 역할을 합니다.
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
