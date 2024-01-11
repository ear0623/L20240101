// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_UpdateUserName, const FString&, UserName);

/**
 * 
 */
UCLASS()
class SHOOTINGCODEGAME_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void BeginPlay()override;
public:
	UPROPERTY(ReplicatedUsing = OnRep_UserName)
	FString m_UserName;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateUserName m_Dele_UpdateUserName;

	UFUNCTION()
	void OnRep_UserName();

public:
	void SetUserName(const FString& UserName);

	void UpdateBind();

	FTimerHandle th_handler;
};
