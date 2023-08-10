#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KartTestModel.generated.h"

UCLASS()
class MARIOKART_API AKartTestModel : public ACharacter
{
	GENERATED_BODY()

public:
	AKartTestModel();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USceneComponent* BaseScene;
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UCameraComponent* TestCamera;
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UStaticMeshComponent* KartBody;
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* CharacterBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Value)
	float HorizontalValue;


public:
	void Horizontal(float Value);
};
