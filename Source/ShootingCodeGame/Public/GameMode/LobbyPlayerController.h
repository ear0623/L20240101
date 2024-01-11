// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCODEGAME_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void OnPossess(APawn* InPawn)override;

	UFUNCTION(Client,Reliable)
	void ResClientPossess();

	UFUNCTION(Server,Reliable)
	void ReqChangeUserName(const FString& NewName);//스트링은 사이즈가 크다. 그래서 래퍼런스 주소로 넘겨주는게 효율적이다.
};
