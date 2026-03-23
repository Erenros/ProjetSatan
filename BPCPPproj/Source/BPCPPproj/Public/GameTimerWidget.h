//#pragma once
//
//#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
//#include "GameTimerWidget.generated.h"
//
//class UTextBlock;
//
//UCLASS()
//class BPCPPPROJ_API UGameTimerWidget : public UUserWidget
//{
//	GENERATED_BODY()
//
//public:
//	virtual void NativeConstruct() override;
//
//protected:
//	void UpdateTimer();
//
//	int32 TotalSeconds = 0;
//	FTimerHandle TimerHandle;
//
//	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
//	void OnTimerUpdated(const FString& TimeString);
//};