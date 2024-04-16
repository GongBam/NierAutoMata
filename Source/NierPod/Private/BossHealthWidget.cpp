

#include "BossHealthWidget.h"
#include "BossCharacter.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ProgressBar.h>

void UBossHealthWidget::NativeTick(const FGeometry& BossGeometry, float DeltaSeconds)
{
	if (bossCharacter != nullptr)
	{
		BossHealthBar->SetPercent(bossCharacter->GetHealth() / bossCharacter->GetMaxHealth());
	}

}
