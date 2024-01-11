// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME 사용을 위해 추가
#include "GameMode/LobbyHUD.h"

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 아래의 함수를 사용해서 서버에서 동기화를 시켜줍니다.
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
		}//로비스테이츠가 업데이트될때마다. 자기자신을 바인딩.즉 모든
	}

	FTimerManager& Timer = GetWorldTimerManager();
	Timer.SetTimer(th_handler, this, &ALobbyPlayerState::UpdateBind, 0.01f, false);
}
