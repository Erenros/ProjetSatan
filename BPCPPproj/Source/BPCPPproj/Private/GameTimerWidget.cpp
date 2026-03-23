//#include "GameTimerWidget.h"
//#include "Components/TextBlock.h"
//#include "TimerManager.h"
//
//void UGameTimerWidget::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	// Timer Start
//	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGameTimerWidget::UpdateTimer, 1.0f, true);
//
//	TotalSeconds = 0;
//	UpdateTimer();
//}
//
//void UGameTimerWidget::UpdateTimer()
//{
//	TotalSeconds++;
//
//	int32 Mins = TotalSeconds / 60;
//	int32 Secs = TotalSeconds % 60;
//
//	// %02d = "Chiffress tjr avec un 0 devant parceque il faut 2 caractères"  5 = "05".
//	FString TimeString = FString::Printf(TEXT("%02d:%02d"), Mins, Secs);
//
//	// Updt du texteblock
//	OnTimerUpdated(TimeString);
//}