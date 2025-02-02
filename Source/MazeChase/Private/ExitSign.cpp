// Marcos Vázquez. ESAT 2017.

#include "MazeChase.h"
#include "ExitSign.h"


// Sets default values
AExitSign::AExitSign() {
	PrimaryActorTick.bCanEverTick = true;
	
	mesh_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh_->AttachTo(RootComponent);

	collider_ = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	collider_->AttachTo(mesh_);

	collider_->OnComponentBeginOverlap.AddDynamic(this, &AExitSign::OnBoxBeginOverlap);
}


void AExitSign::BeginPlay() {
	Super::BeginPlay();
}


void AExitSign::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
}


void AExitSign::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AMazeChaseGameMode* gm = Cast<AMazeChaseGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(gm) && !gm->game_won_) {
		AMazeChaseCharacter* player = Cast<AMazeChaseCharacter>(OtherActor);
		if (IsValid(player)) {
			//Remove all widgets
			UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());

			//Add game won widget
			UUserWidget_GameEnd* WidgetInstance = CreateWidget<UUserWidget_GameEnd>(GetWorld(), game_won_widget_class_);
			WidgetInstance->AddToViewport();

			//Set time passed in widget
			int time = FMath::TruncToInt(GetGameTimeSinceCreation());
			WidgetInstance->setTime(time);

			//Mark game won
			gm->game_won_ = true;

			//Fade out to menu
			gm->fadeToMenu();
		}
	}
}