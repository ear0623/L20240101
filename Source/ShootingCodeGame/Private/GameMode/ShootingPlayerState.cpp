// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShootingPlayerState.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME 사용을 위해 추가

// 생성자에 HP 초기값 지정
AShootingPlayerState::AShootingPlayerState():m_CurHp(100),m_Mag(0),m_Heal(100)
{

}

void AShootingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 아래의 함수를 사용해서 서버에서 동기화를 시켜줍니다.
	DOREPLIFETIME(AShootingPlayerState, m_CurHp);
	DOREPLIFETIME(AShootingPlayerState, m_Mag);
}

void AShootingPlayerState::AddDamage(float Damage)
{
	// 변화값 적용한 다음에 OnRep_CurHp 함수를 호출해줘야 실제로 작동하게 됩니다.
	m_CurHp = m_CurHp - Damage;

	OnRep_CurHp();
}

void AShootingPlayerState::AddMag()
{
	m_Mag = m_Mag + 1.0f;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("AddMag :%f"),m_Mag));
	OnRep_Mag();
}

void AShootingPlayerState::AddHeal()
{
	if (m_CurHp < 100)
	{
		m_CurHp= m_Heal;
	}
	else
	{
		m_CurHp;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("OnRep_CurHp = %f"), m_CurHp));
	OnRep_Heal();
	OnRep_CurHp();

}

int AShootingPlayerState::UseMag(int Ammo)
{
	m_Ammo = Ammo;

	if (Ammo<30 && m_Mag >0)
	{
		for (int i = 0; i < m_Mag; i++)
		{
			if (Ammo < 30)
			{
				m_Ammo = Ammo + 1;
				m_Mag= m_Mag -1;
				
			}
			else
			{
			}
			
		}
		OnRep_Mag();
		return m_Ammo;
	}
	else
	{
		return m_Ammo;
	}
}

void AShootingPlayerState::OnRep_CurHp()
{
	// 출력문을 통해서 변화값을 확인하기 위해서 아래처럼 작성해줍니다.
	// Print 문 안의 %f 는 m_CurHp 의 값이 됩니다.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("OnRep_CurHp = %f"), m_CurHp));

	// 바인드가 걸려 있을때만 함수를 호출시키기 위한 조건문을 추가해줍니다.
	if (m_Dele_UpdateHp.IsBound())
	{
		m_Dele_UpdateHp.Broadcast(m_CurHp, 100);
	}
}

void AShootingPlayerState::OnRep_Mag()
{
	if (m_Dele_UpdateMag.IsBound())
	{
		m_Dele_UpdateMag.Broadcast(m_Mag);
	}
}

void AShootingPlayerState::OnRep_Heal()
{
	if (m_Dele_UpdateHeal.IsBound())
	{
		m_Dele_UpdateHeal.Broadcast(m_CurHp, 100);
	}
}
