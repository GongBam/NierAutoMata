

#include "GameStart.h"

void UGameStart::EnableUserInput(bool bEnable)
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (playerController != nullptr)
	{
		APawn* playerPawn = playerController->GetPawn();

		if (playerPawn != nullptr)
		{
			if (bEnable)
			{
				playerPawn->EnableInput(playerController);
			}
			else
			{
				playerPawn->DisableInput(playerController);
			}			
		}		
	}	
}
