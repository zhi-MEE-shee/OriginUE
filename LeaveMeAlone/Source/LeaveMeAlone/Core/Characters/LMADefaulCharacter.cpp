// Leave Me Alone Game. Some rights reserved.


#include "Core/Characters/LMADefaulCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	
}

