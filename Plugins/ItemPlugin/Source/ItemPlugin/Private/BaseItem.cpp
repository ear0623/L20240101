// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ������� ���� ���� StaticMesh �� Mesh ��� �̸����� StaticMeshComponent �� �߰��մϴ�.
	// ���Ŀ� �ش� StaticMesh �� �ùķ���Ʈ �������� Ȱ��ȭ ���ݴϴ�.
	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	m_Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	// StaticMesh �� Ʈ���������� �ֻ��� ������Ʈ�� ���ݴϴ�.
	SetRootComponent(m_StaticMesh);
	SetRootComponent(m_Scene);
	m_StaticMesh->SetupAttachment(m_Scene);
	m_StaticMesh->SetCollisionProfileName("OverlapAllDynamic");


	// ���ø�����Ʈ �ɼ��� Ȱ��ȭ ���ְ� Movement ���ø�����Ʈ�� Ȱ��ȭ ���ݴϴ�.
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

