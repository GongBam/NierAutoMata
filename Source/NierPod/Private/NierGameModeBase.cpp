// Fill out your copyright notice in the Description page of Project Settings.


#include "NierGameModeBase.h"
#include "BossHealthWidget.h"
#include "GameStart.h"
#include "GameOver.h"
#include <TimerManager.h>

void ANierGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 파일이 할당되어 있다면, 그 위젯 파일을 인스턴스로 생성한다.
// 	if (mainWidget_BP != nullptr)
// 	{
// 		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget_BP);
// 
// 		// 위젯 파일이 제대로 생성되었다면, 위젯 인스턴스를 뷰포트에 추가한다.
// 		if (mainUI != nullptr)
// 		{
// 			mainUI->AddToViewport();
// 
// 			// text_currentPoint 변수의 text 값을 "0"으로 초기화 해준다.
// 			mainUI->text_currentPoint->SetText(FText::FromString("0"));
// 
// 			// 파일에 있는 최고 점수 값을 읽어서 bestPoint 변수에 넣는다.
// 			FString loadPoint;
// 			FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestScore.txt");
// 			FFileHelper::LoadFileToString(loadPoint, *fullPath);
// 			bestPoint = FCString::Atoi(*loadPoint);
// 
// 			// bestPoint 변수의 값을 출력한다.
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
			// 게임 오버 UI를 화면에 그린다.
			gameOverUI->AddToViewport();

			// 마우스 커서를 화면에 보이게 처리한다.
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

			// 메인 UI는 화면에서 제거한다.
			//mainUI->RemoveFromParent();

			// 메인 UI의 점수 표시를 화면에서 보이지 않게 숨겨놓는다.
			//mainUI->ShowCurrentPointText(false);

			// 게임을 일시 정지 상태로 놓는다.
			// 월드의 프레임 시간 배율을 0으로 바꾼다.
			GetWorldSettings()->SetTimeDilation(0);
		}
	}
}

void ANierGameModeBase::HideGameOverUI()
{
	// 월드의 시간 배율을 1배속으로 변경한다.
	GetWorldSettings()->SetTimeDilation(1.0f);

	// 마우스 커서를 안보이게 한다.
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

	// 게임 오버 위젯을 제거한다.
	gameOverUI->RemoveFromParent();

	// 메인 UI 위젯을 다시 생성한다.
	// mainUI->AddToViewport();
}