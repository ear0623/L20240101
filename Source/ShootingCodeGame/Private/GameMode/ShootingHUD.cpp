// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShootingHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/ShootingPlayerState.h"

void AShootingHUD::BeginPlay()
{
	Super::BeginPlay();

	// 이렇게 해주면 에러가 발생할 경우 위치를 알려줍니다.
	check(HudWidgetClass);

	HudWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);
	HudWidget->AddToViewport();

	BindMyPlayerState();
}

void AShootingHUD::BindMyPlayerState()
{
	// GetFirstPlayerController 이 PlayerController 에서 0 번째를 가져오는 방법입니다.
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	
	if (IsValid(pc))
	{
		// 캐릭터의 PlayerState 를 형변환을 통해 가져옵니다.
		AShootingPlayerState* ps = Cast<AShootingPlayerState>(pc->PlayerState);
		if (IsValid(ps))
		{
			// 가져온 PlayerState 인 ps 에 바인딩 해줄 이벤트를 지정해줍니다.(주소로 함수명을 겁니다)
			ps->m_Dele_UpdateHp.AddDynamic(this, &AShootingHUD::OnUpdateMyHp);

			// 바인딩 성공시 초기 HP 값 세팅
			OnUpdateMyHp(ps->m_CurHp, 100);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("AShootingHUD Bind Success!"));
			return;
		}
	}

	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	// 타이머 세팅 - 타이머 변수, 하는 행위 PlayerController 0번 즉 자기 자신, 시간은 0.01초 후에, 반복은 하지 않음
	timerManager.SetTimer(th_BindMyPlayerState, this, &AShootingHUD::BindMyPlayerState, 0.01f, false);
}

void AShootingHUD::OnUpdateMyHp_Implementation(float CurHp, float MaxHp)
{

}
