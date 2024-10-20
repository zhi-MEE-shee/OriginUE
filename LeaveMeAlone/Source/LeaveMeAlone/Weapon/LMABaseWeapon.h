// Leave Me Alone Game. Some rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

class USkeletalMeshComponent;
DECLARE_MULTICAST_DELEGATE(FOnNotifyClipIsEmpty);
DECLARE_MULTICAST_DELEGATE(FOnNotifyOpenedFire);

USTRUCT(BlueprintType)
struct FAmmoWeapon
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite;

};

UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMABaseWeapon();

	void Fire();
	void EndFire();   
	void ChangeClip();
	bool IsCurrentClipEmpty() const;
	bool IsCurrentClipFull() const;

	FOnNotifyClipIsEmpty OnNotifyClipIsEmpty;
	FOnNotifyOpenedFire OnNotifyOpenedFire;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon AmmoWeapon{ 30, 1, true };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	float TraceDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName MazzelSocket = "Muzzle";

	float ShootRate = 0.3f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Shoot();
	void DecrementBullets();
	void DrawTrace();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FAmmoWeapon CurrentAmmoWeapon;
	FTimerHandle TimeToShoot;

};
