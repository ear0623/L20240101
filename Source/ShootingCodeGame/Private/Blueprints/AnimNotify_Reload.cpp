// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprints/AnimNotify_Reload.h"
#include "GameMode/ShootingCodeGameCharacter.h"
#include "Blueprints/WeaponInterface.h"

void UAnimNotify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// 형변환을 통해서 캐릭터를 가져오고
	AShootingCodeGameCharacter* pChar = Cast<AShootingCodeGameCharacter>(MeshComp->GetOwner());
	if (false == IsValid(pChar))
		return;

	// 이후 해당 캐릭터의 무기 정보를 형변환을 통해 가져옵니다.
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(pChar->m_EquipWeapon);
	if (InterfaceObj == nullptr)
		return;

	// 이후에 인터페이스로부터 EventShoot 를 호출하게 됩니다.
	InterfaceObj->Execute_EventResetAmmo(pChar->m_EquipWeapon);
}
