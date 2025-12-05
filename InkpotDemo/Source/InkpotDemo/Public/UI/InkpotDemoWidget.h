#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InkpotDemoWidget.generated.h"

class UInkpotStory;

UCLASS(Abstract, Blueprintable)
class INKPOTDEMO_API UInkpotDemoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="InkpotDemo")
	void ShowStory( UInkpotStory *Story );

	UFUNCTION(BlueprintImplementableEvent, Category="InkpotDemo", meta=(DisplayName="OnShowStory"))
	void ReceiveOnShowStory( UInkpotStory *Story );

private:
	UPROPERTY(VisibleAnywhere, Transient )
	TObjectPtr<UInkpotStory> InkStory{nullptr};
};
