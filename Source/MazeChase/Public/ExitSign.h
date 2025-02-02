// Marcos Vázquez. ESAT 2017.

#pragma once

#include "GameFramework/Actor.h"

#include "WidgetLayoutLibrary.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Core/UI/UserWidget_GameEnd.h"

#include "../MazeChaseGameMode.h"
#include "MazeChaseCharacter.h"
#include "ExitSign.generated.h"

UCLASS()
class MAZECHASE_API AExitSign : public AActor
{
	GENERATED_BODY()
	
public:	
	AExitSign();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UStaticMeshComponent* mesh_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UBoxComponent* collider_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Refs")
	TSubclassOf<UUserWidget_GameEnd> game_won_widget_class_;
};
