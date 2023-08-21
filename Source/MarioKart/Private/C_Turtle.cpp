// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Turtle.h"
#include "Components/BoxComponent.h"

// Sets default values
AC_Turtle::AC_Turtle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = BoxCollision;
	BoxCollision->SetCollisionProfileName(TEXT("Turtle"));

	// 메시 컴포넌트 생성 및 연결
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxCollision);
	// 충돌 설정 변경
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 스케일 배수 조정
	FVector NewScale = FVector(8.0f, 8.0f, 8.0f);
	BoxCollision->SetWorldScale3D(NewScale);


	// 메시 컴포넌트의 메시를 설정 
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/3_SM/Stadium_Change/Item/Scap/SM_ItemP_red_scab.SM_ItemP_red_scab'"));
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
	}
	MeshComponent->SetWorldScale3D(FVector(0.5f));
}

// Called when the game starts or when spawned
void AC_Turtle::BeginPlay()
{
	Super::BeginPlay();

	// 메시 컴포넌트의 충돌 이벤트를 바인딩합니다.
	MeshComponent->OnComponentHit.AddDynamic(this, &AC_Turtle::OnHit);
}

// Called every frame
void AC_Turtle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{
		FVector P0 = GetActorLocation();
		// 방향이 필요하다.
		FVector vt = GetActorForwardVector() * speed * DeltaTime;
		// 이동하고 싶다.
		FVector P = P0 + vt;
		SetActorLocation(P);

	}
}

void AC_Turtle::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FString DebugMessage = TEXT("OnHit function called!");
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, DebugMessage);

	// 다른 액터와 부딪혔을 때 처리 로직을 추가합니다.
	FVector ReflectVector = FMath::GetReflectionVector(GetActorForwardVector(), Hit.ImpactNormal);
	SetActorRotation(ReflectVector.Rotation()); // 현재 방향을 반사된 벡터의 방향으로 설정

	// 부딪힌 후 위치를 이동합니다.
	FVector NewLocation = GetActorLocation() + ReflectVector * speed * GetWorld()->GetDeltaSeconds();
	SetActorLocation(NewLocation);

	FString strState = TEXT("Hit Detected"); // strState 값을 설정
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, strState);
}

