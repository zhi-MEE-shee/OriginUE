// Leave Me Alone Game. Some rights reserved.


#include "Core/Controllers/LMAPlayerController.h"

void ALMAPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}
