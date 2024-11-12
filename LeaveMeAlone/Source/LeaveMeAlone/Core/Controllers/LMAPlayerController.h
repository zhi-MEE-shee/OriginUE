// Leave Me Alone Game. Some rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LMAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LEAVEMEALONE_API ALMAPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	virtual void OnUnPossess() override;

protected:
	virtual void BeginSpectatingState();

};
