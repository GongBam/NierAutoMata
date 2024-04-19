// Fill out your copyright notice in the Description page of Project Settings.


#include "NierGameModeBase.h"
#include "BossHealthWidget.h"
#include "GameStart.h"
#include "GameOver.h"
#include <TimerManager.h>

void ANierGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// ���� ������ �Ҵ�Ǿ� �ִٸ�, �� ���� ������ �ν��Ͻ��� �����Ѵ�.
// 	if (mainWidget_BP != nullptr)
// 	{
// 		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget_BP);
// 
// 		// ���� ������ ����� �����Ǿ��ٸ�, ���� �ν��Ͻ��� ����Ʈ�� �߰��Ѵ�.
// 		if (mainUI != nullptr)
// 		{
// 			mainUI->AddToViewport();
// 
// 			// text_currentPoint ������ text ���� "0"���� �ʱ�ȭ ���ش�.
// 			mainUI->text_currentPoint->SetText(FText::FromString("0"));
// 
// 			// ���Ͽ� �ִ� �ְ� ���� ���� �о bestPoint ������ �ִ´�.
// 			FString loadPoint;
// 			FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestScore.txt");
// 			FFileHelper::LoadFileToString(loadPoint, *fullPath);
// 			bestPoint = FCString::Atoi(*loadPoint);
// 
// 			// bestPoint ������ ���� ����Ѵ�.
// 			mainUI->text_bestPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), bestPoint)));
// 		}
// 	}

	gameStartUI = CreateWidget<UGameStart>(GetWorld(), gameStartWidget_bp);
	if (gameStartUI != nullptr)
	{
		gameStartUI->AddToViewport();

		gameStartUI->EnableUserInput(false);

		FTimerHandle releaseTimerHandle;
		float GravityTime = 3.0f;

		//GetWorld()->GetTimerManager().SetTimer(releaseTimerHandle, FTimerDelegate::CreateLambda([&](){}), GravityTime, false);

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

// void ANierGameModeBase::Tick(float DeltaTime)
// {
// 
// 
// }


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

			// ���� UI�� ȭ�鿡�� �����Ѵ�.
			//mainUI->RemoveFromParent();

			// ���� UI�� ���� ǥ�ø� ȭ�鿡�� ������ �ʰ� ���ܳ��´�.
			//mainUI->ShowCurrentPointText(false);

			// ������ �Ͻ� ���� ���·� ���´�.
			// ������ ������ �ð� ������ 0���� �ٲ۴�.
			GetWorldSettings()->SetTimeDilation(0);
		}
	}
}

void ANierGameModeBase::HideGameOverUI()
{
	// ������ �ð� ������ 1������� �����Ѵ�.
	GetWorldSettings()->SetTimeDilation(1.0f);

	// ���콺 Ŀ���� �Ⱥ��̰� �Ѵ�.
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

	// ���� ���� ������ �����Ѵ�.
	gameOverUI->RemoveFromParent();

	// ���� UI ������ �ٽ� �����Ѵ�.
	// mainUI->AddToViewport();
}