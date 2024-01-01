// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShootingHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/ShootingPlayerState.h"

void AShootingHUD::BeginPlay()
{
	Super::BeginPlay();

	// �̷��� ���ָ� ������ �߻��� ��� ��ġ�� �˷��ݴϴ�.
	check(HudWidgetClass);

	HudWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);
	HudWidget->AddToViewport();

	BindMyPlayerState();
}

void AShootingHUD::BindMyPlayerState()
{
	// GetFirstPlayerController �� PlayerController ���� 0 ��°�� �������� ����Դϴ�.
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	
	if (IsValid(pc))
	{
		// ĳ������ PlayerState �� ����ȯ�� ���� �����ɴϴ�.
		AShootingPlayerState* ps = Cast<AShootingPlayerState>(pc->PlayerState);
		if (IsValid(ps))
		{
			// ������ PlayerState �� ps �� ���ε� ���� �̺�Ʈ�� �������ݴϴ�.(�ּҷ� �Լ����� �̴ϴ�)
			ps->m_Dele_UpdateHp.AddDynamic(this, &AShootingHUD::OnUpdateMyHp);

			// ���ε� ������ �ʱ� HP �� ����
			OnUpdateMyHp(ps->m_CurHp, 100);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("AShootingHUD Bind Success!"));
			return;
		}
	}

	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	// Ÿ�̸� ���� - Ÿ�̸� ����, �ϴ� ���� PlayerController 0�� �� �ڱ� �ڽ�, �ð��� 0.01�� �Ŀ�, �ݺ��� ���� ����
	timerManager.SetTimer(th_BindMyPlayerState, this, &AShootingHUD::BindMyPlayerState, 0.01f, false);
}

void AShootingHUD::OnUpdateMyHp_Implementation(float CurHp, float MaxHp)
{

}
