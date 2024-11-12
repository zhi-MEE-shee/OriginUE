// Leave Me Alone Game. Some rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaulCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UMaterialInterface;
class ULMAHealthComponent;
class UAnimMontage;
class ULMAWeaponComponent;

UCLASS()
class LEAVEMEALONE_API ALMADefaulCharacter : public ACharacter
{
	GENERATED_BODY()

	//*PROPERTIES*//
public:
	ALMADefaulCharacter();

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintCallable)
	float GetStamina() { return CurrentStamina; };

	UFUNCTION()
	void OnDeath();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial;

	UPROPERTY()
	UDecalComponent* Cursor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULMAWeaponComponent* WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving settings")
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving settings")
	float CurrentStamina = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Moving settings")
	float Velocity = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Moving settings")
	float SprintVelocity = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSprinting = false;

	UPROPERTY(EditDefaultsOnly, Category = "Moving settings")
	float StaminaTick = 10.0;


private:
	float YRotation = -75.0f;
	float ArmLength = 1400.0f;
	float FOV = 55.0;

	float MaxZoom = 2000.0f;
	float MinZoom = 600.0f;
	float ZoomFactor = 50.0f;

	
	FTimerHandle TimerToIncreaseStamina;
	FTimerHandle TimerToDecreaseStamina;
	


	//*FUNCTIONS*//


	void OnHealthChanged(float NewHealth);
	void RotationPlayerOnCursor();

protected:
	virtual void BeginPlay() override;

	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void CameraZoom(float Value);
	virtual void StartSprint();
	virtual void StopSprint();

	virtual void StaminaIncrease();
	virtual void StaminaDecrease();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
