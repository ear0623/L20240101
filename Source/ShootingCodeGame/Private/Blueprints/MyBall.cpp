// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprints/MyBall.h"

// Sets default values
AMyBall::AMyBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 헤더에서 만든 변수 StaticMesh 에 Mesh 라는 이름으로 StaticMeshComponent 를 추가합니다.
	// 이후에 해당 StaticMesh 의 시뮬레이트 피직스를 활성화 해줍니다.
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	StaticMesh->SetSimulatePhysics(true);

	// StaticMesh 를 트리구조에서 최상위 컴포넌트로 해줍니다.
	SetRootComponent(StaticMesh);

	// 리플리케이트 옵션을 활성화 해주고 Movement 리플리케이트도 활성화 해줍니다.
	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AMyBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

