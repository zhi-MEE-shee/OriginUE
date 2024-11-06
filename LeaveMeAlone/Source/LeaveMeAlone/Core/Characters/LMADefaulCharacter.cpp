// Leave Me Alone Game. Some rights reserved.


#include "Core/Characters/LMADefaulCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Components/LMAHealthComponent.h"
#include "Components/LMAWeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"

ALMADefaulCharacter::ALMADefaulCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);


	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->TargetArmLength = ArmLength;
	SpringArm->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	
	Camera->SetFieldOfView(FOV);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");
	CurrentStamina = MaxStamina;

	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");
}




void ALMADefaulCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		if (IsValid(CursorMaterial))
		{
			Cursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
		}
	}
	HealthComponent->OnDeath.AddUObject(this, &ALMADefaulCharacter::OnDeath);
//	OnHealthChanged(HealthComponent->GetHealth());
//	HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaulCharacter::OnHealthChanged);
}

void ALMADefaulCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaulCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaulCharacter::CameraZoom(float Value)
{
	if (Value == 0) { return; }
	
	float NewArmLength = SpringArm->TargetArmLength + Value * ZoomFactor;
	SpringArm->TargetArmLength = FMath::Clamp(NewArmLength, MinZoom, MaxZoom);

}

void ALMADefaulCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HealthComponent->IsAlive()) {
		RotationPlayerOnCursor();
	}
}

void ALMADefaulCharacter::RotationPlayerOnCursor()
{
	if (GetWorld())
	{
		APlayerController* PlayerControler = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PlayerControler))
		{
			FHitResult HitResult;
			PlayerControler->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult);
			float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResult.Location).Yaw;
			SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
			if (Cursor)
			{
				Cursor->SetWorldLocation(HitResult.Location);
			}
		}
	}
}
	

void ALMADefaulCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaulCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaulCharacter::MoveRight);
	PlayerInputComponent->BindAxis("CameraZoom", this, & ALMADefaulCharacter::CameraZoom);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaulCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaulCharacter::StopSprint);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::EndFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Reload);
	
}

void ALMADefaulCharacter::OnDeath()
{
	Cursor->DestroyRenderState_Concurrent();
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ALMADefaulCharacter::OnHealthChanged(float NewHealth)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}

void ALMADefaulCharacter::StartSprint()
{
	if (CurrentStamina > 0 && GetVelocity().Length() > 0) {
		IsSprinting = true;
		if (IsValid(WeaponComponent)) {
			WeaponComponent->SetSprintState(IsSprinting);
			WeaponComponent->EndFire();
		}
		GetCharacterMovement()->MaxWalkSpeed = SprintVelocity;
		GetWorld()->GetTimerManager().SetTimer(TimerToDecreaseStamina, this, &ALMADefaulCharacter::StaminaDecrease, 0.5F, true);
	}
}

void ALMADefaulCharacter::StopSprint()
{
	IsSprinting = false;
	if (IsValid(WeaponComponent)) {
		WeaponComponent->SetSprintState(IsSprinting);
	}
	GetCharacterMovement()->MaxWalkSpeed = Velocity;
	GetWorld()->GetTimerManager().ClearTimer(TimerToDecreaseStamina);
	GetWorld()->GetTimerManager().SetTimer(TimerToIncreaseStamina, this, &ALMADefaulCharacter::StaminaIncrease, 0.5f, true);
}

void ALMADefaulCharacter::StaminaIncrease()
{
	CurrentStamina = FMath::Clamp(CurrentStamina + StaminaTick, 0.0f, MaxStamina);
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, FString::Printf(TEXT("Stamina = %f"), CurrentStamina));	
	if (CurrentStamina == MaxStamina)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerToIncreaseStamina);
	}
}

void ALMADefaulCharacter::StaminaDecrease()
{
	CurrentStamina = FMath::Clamp(CurrentStamina - (StaminaTick * 2), 0.0f, MaxStamina);
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, FString::Printf(TEXT("Stamina = %f"), CurrentStamina));

	if (CurrentStamina < 0.001 || FMath::IsNearlyZero(GetVelocity().Length()))
	{
		StopSprint();
	}

}