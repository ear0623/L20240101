// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShootingPlayerState.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME ����� ���� �߰�

// �����ڿ� HP �ʱⰪ ����
AShootingPlayerState::AShootingPlayerState():m_CurHp(100),m_Mag(0),m_Heal(100)
{

}

void AShootingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// �Ʒ��� �Լ��� ����ؼ� �������� ����ȭ�� �����ݴϴ�.
	DOREPLIFETIME(AShootingPlayerState, m_CurHp);
	DOREPLIFETIME(AShootingPlayerState, m_Mag);
}

void AShootingPlayerState::AddDamage(float Damage)
{
	// ��ȭ�� ������ ������ OnRep_CurHp �Լ��� ȣ������� ������ �۵��ϰ� �˴ϴ�.
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
	// ��¹��� ���ؼ� ��ȭ���� Ȯ���ϱ� ���ؼ� �Ʒ�ó�� �ۼ����ݴϴ�.
	// Print �� ���� %f �� m_CurHp �� ���� �˴ϴ�.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("OnRep_CurHp = %f"), m_CurHp));

	// ���ε尡 �ɷ� �������� �Լ��� ȣ���Ű�� ���� ���ǹ��� �߰����ݴϴ�.
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
