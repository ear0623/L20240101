// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME ����� ���� �߰�
#include "GameMode/LobbyHUD.h"

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// �Ʒ��� �Լ��� ����ؼ� �������� ����ȭ�� �����ݴϴ�.
	DOREPLIFETIME(ALobbyPlayerState, m_UserName);
}

void ALobbyPlayerState::BeginPlay()
{
	Super::BeginPlay();

	UpdateBind();
}

void ALobbyPlayerState::OnRep_UserName()
{
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::White, FString::Printf(TEXT("Client OnRep_UseName=%s"),*m_UserName));

	if (m_Dele_UpdateUserName.IsBound())
	{
		m_Dele_UpdateUserName.Broadcast(m_UserName);
	}
}

void ALobbyPlayerState::SetUserName(const FString& UserName)
{
	m_UserName = UserName;

	OnRep_UserName();
}

void ALobbyPlayerState::UpdateBind()
{
	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();
	if (pPlayer0 == nullptr)
	{
		ALobbyHUD* pHud = Cast<ALobbyHUD>(pPlayer0->GetHUD());
		if (pHud)
		{
			pHud->BindPlayerState(this);
			return;
		}//�κ��������� ������Ʈ�ɶ�����. �ڱ��ڽ��� ���ε�.�� ���
	}

	FTimerManager& Timer = GetWorldTimerManager();
	Timer.SetTimer(th_handler, this, &ALobbyPlayerState::UpdateBind, 0.01f, false);
}
