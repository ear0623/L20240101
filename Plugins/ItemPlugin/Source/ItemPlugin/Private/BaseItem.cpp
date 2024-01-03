// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 헤더에서 만든 변수 StaticMesh 에 Mesh 라는 이름으로 StaticMeshComponent 를 추가합니다.
	// 이후에 해당 StaticMesh 의 시뮬레이트 피직스를 활성화 해줍니다.
	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	m_Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	// StaticMesh 를 트리구조에서 최상위 컴포넌트로 해줍니다.
	SetRootComponent(m_StaticMesh);
	SetRootComponent(m_Scene);
	m_StaticMesh->SetupAttachment(m_Scene);
	m_StaticMesh->SetCollisionProfileName("OverlapAllDynamic");


	// 리플리케이트 옵션을 활성화 해주고 Movement 리플리케이트도 활성화 해줍니다.
	bReplicates = true;
	SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_StaticMesh->AddRelativeRotation(FRotator(0.0f, 1.0f, 0.0f));

}

