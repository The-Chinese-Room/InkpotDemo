#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "InkpotDemoListChoice.generated.h"

UCLASS(BlueprintType, Blueprintable)
class INKPOTDEMO_API UInkpotDemoListChoice : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="InkpotDemo", meta=(DisplayName="OnTextUpdate"))
	void ReceiveOnTextUpdate( const FText& InText );

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
