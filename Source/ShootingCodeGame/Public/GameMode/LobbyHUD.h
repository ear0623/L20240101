// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCODEGAME_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

public:

	void BindPlayerState(class ALobbyPlayerState* PlayerState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HudWidgetClass;

	// ������ ������ �����ϱ� ���� ����
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* HudWidget;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateUserName(const FString& UserName);

	void OnUpdateUserName_Implementation(const FString& UserName);
};
