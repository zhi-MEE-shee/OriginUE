// Leave Me Alone Game. Some rights reserved.


#include "LevelActors/PickUp/LMAHealthPickUp.h"
#include "Components/LMAHealthComponent.h"
#include "Components/SphereComponent.h"
#include "Core/Characters/LMADefaulCharacter.h"

// Sets default values
ALMAHealthPickUp::ALMAHealthPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(SphereComponent);
}

// Called when the game starts or when spawned
void ALMAHealthPickUp::BeginPlay()
{
	Super::BeginPlay();	
}

void ALMAHealthPickUp::PickupWasTaken()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetRootComponent()->SetVisibility(false, true);
	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ALMAHealthPickUp::RespawnPickup, RespawnTime);
}

void ALMAHealthPickUp::RespawnPickup()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	GetRootComponent()->SetVisibility(true, true);
}

bool ALMAHealthPickUp::GivePickup(ALMADefaulCharacter* Character)
{
	const auto HealthComponent = Character->GetHealthComponent();
	if (!HealthComponent) {
		return false;
	}
	return HealthComponent->AddHealth(HealthFromPickup);
}

void ALMAHealthPickUp::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	const auto Charcter = Cast<ALMADefaulCharacter>(OtherActor);
	if (GivePickup(Charcter))
	{
		PickupWasTaken();
	}
}

// Called every frame
void ALMAHealthPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

