// Leave Me Alone Game. Some rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/LMABaseWeapon.h"
#include "LMAWeaponComponent.generated.h"

class ALMABaseWeapon;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULMAWeaponComponent();

	void Fire();
	void EndFire();
	void Reload();
	void SetSprintState(bool _IsSprinting);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ALMABaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* FireMontage;

	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	//Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY()
	ALMABaseWeapon* Weapon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName rHandSocket = "r_Weapon_Socket";

	bool AnimReloading = false;
	bool IsSprinting = false;
	bool IsShooting = false;

	void SpawnWeapon();
	void InitAnimNotify();
	void OnNotifyClipIsEmpty();
	void OnFireMontage();
	void OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh);
	bool CanReload() const;

};
