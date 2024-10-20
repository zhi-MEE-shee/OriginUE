// Leave Me Alone Game. Some rights reserved.

#include "Weapon/LMABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);


// Sets default values
ALMABaseWeapon::ALMABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);

}


// Called when the game starts or when spawned
void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmoWeapon = AmmoWeapon;
}

// Called every frame
void ALMABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALMABaseWeapon::Shoot()
{
	if (!IsCurrentClipEmpty()) {
		OnNotifyOpenedFire.Broadcast();
		DrawTrace();
		DecrementBullets();
	}
}

void ALMABaseWeapon::DrawTrace()
{
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform(MazzelSocket);
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
	}
}



void ALMABaseWeapon::Fire()
{
	if (!GetWorldTimerManager().IsTimerActive(TimeToShoot)) {
		GetWorldTimerManager().SetTimer(TimeToShoot, this, &ALMABaseWeapon::Shoot, ShootRate, true);
	}
}

void ALMABaseWeapon::ChangeClip()
{
	/*if (!CurrentAmmoWeapon.Infinite) {
		if (CurrentAmmoWeapon.Clips > 0) {
			CurrentAmmoWeapon.Clips--;
		}
		// to do
		// else ��������� �� ���������� �����
	}*/
	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
}

bool ALMABaseWeapon::IsCurrentClipFull() const
{
	return CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets;
}

void ALMABaseWeapon::DecrementBullets()
{
	CurrentAmmoWeapon.Bullets--;
	UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));
	if (IsCurrentClipEmpty())
	{
		OnNotifyClipIsEmpty.Broadcast();
	}
}


void ALMABaseWeapon::EndFire()
{
	GetWorldTimerManager().ClearTimer(TimeToShoot);
}
