#include "SpartaGameState.h"

#include "CoinItem.h"
#include "SpartaCharacter.h"
#include "SpartaGameInstance.h"
#include "SpartaPlayerController.h"
#include "SpawnVolume.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0; // 전체 초기화
	LevelDuration = { 30.0f, 35.0f, 40.0f };
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	CurrentWaveIndex = 0;
	MaxWaves = 3;
	ItemToSpawnByWave = { 40, 45, 50 };
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();
	
	StartLevel();
	
	
	// 0.1초마다 남은 시간을 갱신하기 위해 UpdateHUD 호출
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void ASpartaGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}
	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}

	StartWave();
	//TArray<AActor*> FoundVolumes;
	// 월드에서 ASpawnVolume을 찾아 FoundVolumes에 넣어라
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
}

void ASpartaGameState::StartWave()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start!"), CurrentWaveIndex + 1);
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	
	// 코인이 40개가 될때까지 스폰하는 로직
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0; // 웨이브 시작 때마다 초기화
	SpawnItems.Empty(); // 웨이브 시작 때 엑터 비우기
	
	const int32 ItemToSpawn = ItemToSpawnByWave[CurrentWaveIndex];
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			// 스폰 볼륨이 여러개라면 볼륨을 랜덤으로 고르는 것도 하나의 방법이다.
			if (ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]))
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnItems.Add(SpawnedActor);
					SpawnedCoinCount++;
				}
			}
		}
	}
	
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeUp,
		LevelDuration[CurrentWaveIndex],
		false
	);
}

void ASpartaGameState::OnLevelTimeUp()
{
	if (CurrentWaveIndex >= MaxWaves - 1)
	{
		EndLevel();
	}
	else
	{	
		EndWave();
		StartWave();
	}
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount
	);
	
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		if (CurrentWaveIndex >= MaxWaves)
		{
			EndLevel();
		}
		else
		{
			EndWave();
			StartWave();
		}
	}
}

void ASpartaGameState::EndWave()
{
	for (AActor* Item : SpawnItems)
	{
		if (IsValid(Item))
		{
			Item->Destroy();
		}
	}
	SpawnItems.Empty();
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	
	CurrentWaveIndex++;
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer((LevelTimerHandle));
	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	
	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}
	
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		// 월드에서 이름을 가져와 오픈을 해달라
		// 오픈 레벨을 할 때마다 GameState가 다시 만들어 지면서 BeginPlay()가 호출된다.
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver(); // 에러처리 용 게임 오버
	}
}

void ASpartaGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{ // 플레이어 컨트롤러에 있는 위젯에서 텍스트박스 중 이름이 "Time"인걸 얻어옴.
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					// String을 텍스트로 변환해주는 작업
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{ 
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}
				
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{ 
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
						{
							LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
						}
					}	
				}
				
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{ 
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex + 1)));
				}
				
				if (UProgressBar* HPBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HPBar"))))
				{
					APawn* PlayerPawn = SpartaPlayerController->GetPawn();
					
					if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(PlayerPawn))
					{
						if (SpartaCharacter->GetMaxHealth() > 0.0f)
						{
							const float HPRatio = SpartaCharacter->GetHealth() / SpartaCharacter->GetMaxHealth();
							HPBar->SetPercent(HPRatio);
						}
					}
				}
				
				if (UTextBlock* HPText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HP"))))
				{ 
					APawn* PlayerPawn = SpartaPlayerController->GetPawn();
					
					if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(PlayerPawn))
					{
						int32 HP = SpartaCharacter->GetHealth();
						HPText->SetText(FText::FromString(FString::Printf(TEXT("%d"), HP)));
					}
				}
				
				if (UTextBlock* MaxHPText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("MaxHP"))))
				{ 
					APawn* PlayerPawn = SpartaPlayerController->GetPawn();
					
					if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(PlayerPawn))
					{
						int32 MaxHP = SpartaCharacter->GetMaxHealth();
						MaxHPText->SetText(FText::FromString(FString::Printf(TEXT("%d"), MaxHP)));
					}
				}
			}
		}
	}
}
