// Leave Me Alone Game. Some rights reserved.


#include "Components/LMAWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/LMABaseWeapon.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"



// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
	InitAnimNotify();
}


// Called every frame
void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULMAWeaponComponent::SpawnWeapon()
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (IsValid(Weapon)) 
	{
		const auto Character = Cast<ACharacter>(GetOwner());
		if (IsValid(Character))
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, rHandSocket);
		}
	}
	Weapon->OnNotifyOpenedFire.AddUObject(this, &ULMAWeaponComponent::OnFireMontage);
	Weapon->OnNotifyClipIsEmpty.AddUObject(this, &ULMAWeaponComponent::OnNotifyClipIsEmpty);
}

void ULMAWeaponComponent::Fire()
{
	if (IsValid(Weapon) && !AnimReloading && IsSprinting == false)
	{
		Weapon->Fire();
		IsShooting = true;
	}
}

void ULMAWeaponComponent::EndFire() 
{
	if (IsValid(Weapon))
	{
		Weapon->EndFire();
		//IsShooting = false;
	}
}

void ULMAWeaponComponent::InitAnimNotify()
{
	if (!ReloadMontage) return;

	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}


void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
	}
}

void ULMAWeaponComponent::OnFireMontage()
{
	if (!FireMontage) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(FireMontage);
}

void ULMAWeaponComponent::SetSprintState(bool _IsSprinting)
{
	IsSprinting = _IsSprinting;
}


bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading && (!Weapon->IsCurrentClipFull());
}

void ULMAWeaponComponent::OnNotifyClipIsEmpty()
{
	if (!CanReload()) { return; }
	AnimReloading = true;
	if (IsShooting)
	{
		EndFire();
	}
	Weapon->ChangeClip();
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (IsValid(Character))
	{
		Character->PlayAnimMontage(ReloadMontage);
	}
}

void ULMAWeaponComponent::Reload()
{
	OnNotifyClipIsEmpty();
}