// Leave Me Alone Game. Some rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaulCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UMaterialInterface;

UCLASS()
class LEAVEMEALONE_API ALMADefaulCharacter : public ACharacter
{
	GENERATED_BODY()

	//*PROPERTIES*//
public:
	ALMADefaulCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial;

	UPROPERTY()
	UDecalComponent* Cursor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);	

private:
	float YRotation = -75.0f;
	float ArmLength = 1400.0f;
	float FOV = 55.0;

	float MaxZoom = 2000.0f;
	float MinZoom = 600.0f;
	float ZoomFactor = 20.0f;

	//*FUNCTIONS*//
protected:
	virtual void BeginPlay() override;

	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void CameraZoom(float Value);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
