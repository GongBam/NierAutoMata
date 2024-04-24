

#include "NierGameModeBase.h"
#include "BossHealthWidget.h"
#include "GameStart.h"
#include "GameOver.h"
#include "ClearedUIWidget.h"

void ANierGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	

	gameStartUI = CreateWidget<UGameStart>(GetWorld(), gameStartWidget_bp);
	if (gameStartUI != nullptr)
	{
		gameStartUI->AddToViewport();

		gameStartUI->EnableUserInput(false);

		FTimerHandle releaseTimerHandle;
		float GravityTime = 3.0f;

		GetWorld()->GetTimerManager().SetTimer(releaseTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				// 코드 구현
				if (gameStartUI != nullptr)
				{
					gameStartUI->EnableUserInput(true);
					gameStartUI->RemoveFromParent();
				}

				// TimerHandle 초기화
				GetWorld()->GetTimerManager().ClearTimer(releaseTimerHandle);
			}), GravityTime, false);
	}
}
void ANierGameModeBase::ShowGameOverUI()
{	

	if (gameover_BP != nullptr)
	{
		gameOverUI = CreateWidget<UGameOver>(GetWorld(), gameover_BP);

		if (gameOverUI != nullptr)
		{
			// 게임 오버 UI를 화면에 그린다.
			gameOverUI->AddToViewport();

			// 마우스 커서를 화면에 보이게 처리한다.
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			GetWorldSettings()->SetTimeDilation(0);
		}
	}
}

//클리어 UI 띄움
void ANierGameModeBase::ShowClearedUI()
{

	if (clearedWidget_bp != nullptr)
	{
		clearedUI = CreateWidget<UClearedUIWidget>(GetWorld(), clearedWidget_bp);
		if (clearedUI != nullptr)
		{
			clearedUI->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			GetWorldSettings()->SetTimeDilation(0);
		}
	}
}
