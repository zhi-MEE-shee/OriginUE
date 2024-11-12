// Leave Me Alone Game. Some rights reserved.


#include "Core/Controllers/LMAPlayerController.h"
#include "GameFramework/Character.h"
#include "Core/Characters/LMADefaulCharacter.h"
#include "Components/LMAHealthComponent.h"

void ALMAPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void ALMAPlayerController::BeginSpectatingState()
{
	SetControlRotation(FRotator(-75.0f, 0.0f, 0.0f));
	Super::BeginSpectatingState();
}



void ALMAPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	const auto* MyCharacter = Cast<ACharacter, APawn>(GetPawn());
	if (MyCharacter) {
		ULMAHealthComponent* HealthComponent = MyCharacter->FindComponentByClass<ULMAHealthComponent>();
		if (HealthComponent) {
			if (!HealthComponent->IsAlive()) {
			//	HealthComponent->OnDeath.AddDynamic(MyCharacter, &ALMADefaulCharacter::OnDeath);
			}
		}
	}
}