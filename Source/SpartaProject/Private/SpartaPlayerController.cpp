#include "SpartaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "SpartaGameInstance.h"
#include "SpartaGameState.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

ASpartaPlayerController::ASpartaPlayerController()
	: InputMappingContext(nullptr), 
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr), 
	SprintAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr),
	bRestart(false)
{
	
}

bool ASpartaPlayerController::GetRestart()
{
	return bRestart;
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// LocalPlayer == 플레이어의 입력이나 화면 뷰를 관리하는 객체
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		// UEnhancedInputLocalPlayerSubsystem 입력시스템을 관리하는 것
		// IMC을 추가하고 삭제하는 것을 한다
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				// 0은 우선순위를 가장 높게 하라는 뜻. 겹치면 현재 IMC가 활성화 됨.
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
	
	// ShowMainMenu()와 ShowGameHUD()를 구현 했으므로 이제 필요가 없다.
	// if (HUDWidgetClass)
	// {
	// 	// CreateWidget는 UIWidget인스턴스를 생성하는 함수이다.
	// 	HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	// 	if (HUDWidgetInstance)
	// 	{
	// 		HUDWidgetInstance->AddToViewport();
	// 	}
	// }
	
	// // 위젯을 한 번 띄운 직후에 업데이트
	// ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	// if (SpartaGameState)
	// {
	// 	SpartaGameState->UpdateHUD();
	// }
	
	// 현재 띄워져 있는 맵이 메인 메뉴라면 메인 메뉴 UI를 띄워라
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* ASpartaPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ASpartaPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent(); // 부모에서 떼어냄
		HUDWidgetInstance = nullptr;
	}
	
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent(); // 부모에서 떼어냄
		MainMenuWidgetInstance = nullptr;
	}
	
	if (HUDWidgetClass)
	{
		// CreateWidget는 UIWidget인스턴스를 생성하는 함수이다.
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
			// 마우스의 포커스가 게임이 아닌 UI에게만 가도록
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
		
		ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
		if (SpartaGameState)
		{
			SpartaGameState->UpdateHUD();
		}
	}
}

void ASpartaPlayerController::ShowMainMenu(bool bIsRestart)
{
	bRestart = bIsRestart;
	
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent(); // 부모에서 떼어냄
		HUDWidgetInstance = nullptr;
	}
	
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent(); // 부모에서 떼어냄
		MainMenuWidgetInstance = nullptr;
	}
	
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
			// 마우스의 포커스가 게임이 아닌 UI에게만 가도록
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
	
	if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
	{
		if (bRestart)
		{
			ButtonText->SetText(FText::FromString(TEXT("Restart")));
			
		}
		else
		{
			ButtonText->SetText(FText::FromString(TEXT("Start")));
		}
		
		if (bRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}
			
			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score: %d"), SpartaGameInstance->TotalScore))
					);
				}
			}
		}
	}
	
	if (UTextBlock* ExitButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("ExitButtonText"))))
	{
		if (bRestart)
		{
			ExitButtonText->SetText(FText::FromString(TEXT("MainMenu")));
			
		}
		else
		{
			ExitButtonText->SetText(FText::FromString(TEXT("Exit")));
		}
	}
}

void ASpartaPlayerController::StartGame()
{
	if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SpartaGameInstance->CurrentLevelIndex = 0;
		SpartaGameInstance->TotalScore = 0;
	}
	
	// 월드에서 BasicLevel을 오픈해라
	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
}

void ASpartaPlayerController::ExitGame()
{
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}

