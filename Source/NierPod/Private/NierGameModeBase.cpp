

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
				// �ڵ� ����
				if (gameStartUI != nullptr)
				{
					gameStartUI->EnableUserInput(true);
					gameStartUI->RemoveFromParent();
				}

				// TimerHandle �ʱ�ȭ
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
			// ���� ���� UI�� ȭ�鿡 �׸���.
			gameOverUI->AddToViewport();

			// ���콺 Ŀ���� ȭ�鿡 ���̰� ó���Ѵ�.
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			GetWorldSettings()->SetTimeDilation(0);
		}
	}
}

//Ŭ���� UI ���
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
