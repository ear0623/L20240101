// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShootingPlayerState.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME ����� ���� �߰�

// �����ڿ� HP �ʱⰪ ����
AShootingPlayerState::AShootingPlayerState():m_CurHp(100)
{

}

void AShootingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// �Ʒ��� �Լ��� ����ؼ� �������� ����ȭ�� �����ݴϴ�.
	DOREPLIFETIME(AShootingPlayerState, m_CurHp);
}

void AShootingPlayerState::AddDamage(float Damage)
{
	// ��ȭ�� ������ ������ OnRep_CurHp �Լ��� ȣ������� ������ �۵��ϰ� �˴ϴ�.
	m_CurHp = m_CurHp - Damage;

	OnRep_CurHp();
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
