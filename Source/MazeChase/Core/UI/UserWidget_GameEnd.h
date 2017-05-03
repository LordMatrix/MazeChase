// Marcos Vázquez. ESAT 2017.

#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UserWidget_GameEnd.generated.h"

/**
 * 
 */
UCLASS()
class MAZECHASE_API UUserWidget_GameEnd : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "funcs")
		void setTime(int time);

	
	
};
