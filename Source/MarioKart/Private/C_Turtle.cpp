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

	// �޽� ������Ʈ ���� �� ����
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxCollision);
	// �浹 ���� ����
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ������ ��� ����
	FVector NewScale = FVector(8.0f, 8.0f, 8.0f);
	BoxCollision->SetWorldScale3D(NewScale);


	// �޽� ������Ʈ�� �޽ø� ���� 
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

	// �޽� ������Ʈ�� �浹 �̺�Ʈ�� ���ε��մϴ�.
	MeshComponent->OnComponentHit.AddDynamic(this, &AC_Turtle::OnHit);
}

// Called every frame
void AC_Turtle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{
		FVector P0 = GetActorLocation();
		// ������ �ʿ��ϴ�.
		FVector vt = GetActorForwardVector() * speed * DeltaTime;
		// �̵��ϰ� �ʹ�.
		FVector P = P0 + vt;
		SetActorLocation(P);

	}
}

void AC_Turtle::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FString DebugMessage = TEXT("OnHit function called!");
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, DebugMessage);

	// �ٸ� ���Ϳ� �ε����� �� ó�� ������ �߰��մϴ�.
	FVector ReflectVector = FMath::GetReflectionVector(GetActorForwardVector(), Hit.ImpactNormal);
	SetActorRotation(ReflectVector.Rotation()); // ���� ������ �ݻ�� ������ �������� ����

	// �ε��� �� ��ġ�� �̵��մϴ�.
	FVector NewLocation = GetActorLocation() + ReflectVector * speed * GetWorld()->GetDeltaSeconds();
	SetActorLocation(NewLocation);

	FString strState = TEXT("Hit Detected"); // strState ���� ����
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, strState);
}

