// Fill out your copyright notice in the Description page of Project Settings.


#include "ValGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "ValAICharacter.h"
#include "ValAttributeComponent.h"
#include <EngineUtils.h>
#include <DrawDebugHelpers.h>
#include "ValCharacter.h"
#include "ValSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "ValGameplayInterface.h"
#include <Serialization/ObjectAndNameAsStringProxyArchive.h>

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("val.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

AValGameModeBase::AValGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	NumPotionsAndCreditsToSpawn = 10;

	SlotName = "SaveGame01";
}

void AValGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void AValGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AValCharacter* Player = Cast<AValCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	AValAICharacter* Minion = Cast<AValAICharacter>(VictimActor);
	if (Minion)
	{
		AValCharacter* PlayerAsKiller = Cast<AValCharacter>(Killer);
		if (PlayerAsKiller)
		{
			PlayerAsKiller->GrantCredits(Minion->GetCreditForKill());
			UE_LOG(LogTemp, Log, TEXT("The player currently has %i credits."), PlayerAsKiller->GetCredits());
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor),
		*GetNameSafe(Killer));
}

void AValGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether its allowed to spawn determined by spawn logic later in the chain...
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AValGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	SpawnCollectibles();
}

void AValGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	AValPlayerState* PS = NewPlayer->GetPlayerState<AValPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
}

void AValGameModeBase::KillAll()
{
	for (TActorIterator<AValAICharacter> It(GetWorld()); It; ++It)
	{
		AValAICharacter* Bot = *It;

		UValAttributeComponent* AttributeComp = UValAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // @fixme: pass in player? for kill credit
		}
	}	
}

void AValGameModeBase::SpawnCollectibles()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,
		SpawnBotQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AValGameModeBase::OnSpawnCollectibleQueryCompleted);
	}
}

void AValGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}

	int32 NrOfAliveBots = 0;
	for (TActorIterator<AValAICharacter> It(GetWorld()); It; ++It)
	{
		AValAICharacter* Bot = *It;

		UValAttributeComponent* AttributeComp = UValAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrOfAliveBots);

	float MaxBotCount = 1.0f;
	
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity, Skipping bot spawn."));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,
		SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AValGameModeBase::OnSpawnBotQueryCompleted);
	}
}

void AValGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}


	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		// Track all the used spawn locations
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void AValGameModeBase::OnSpawnCollectibleQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Collectibles EQS Query Failed!"));
		return;
	}


	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	TSubclassOf<AActor> ClassToSpawn = HealthPotionClass;

	//UE_LOG(LogTemp, Warning, TEXT("There are %i locations"), Locations.Num());

	int NumSpawned = 0;
	for (auto Location : Locations) {
		Location.Z += 50;
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, Location, FRotator::ZeroRotator);

		// Alternate between spawning health potions and coins
		ClassToSpawn = ClassToSpawn == HealthPotionClass ? CoinClass : HealthPotionClass;

		NumSpawned++;
		if (NumSpawned == NumPotionsAndCreditsToSpawn*2)
		{
			break;
		}
	}
}

void AValGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void AValGameModeBase::WriteSaveGame()
{
	// Iterate all player states, we don't have proper ID to match yet (requires Steam or EOS)
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AValPlayerState* PS = Cast<AValPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // Single player only at this point
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	// Iterate the entire world of actors
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// Only interested in our 'gameplay actors'
		if (!Actor->Implements<UValGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);

		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;

		// Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AValGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UValSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));
		
		// Iterate the entire world of actors
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// Only interested in our 'gameplay actors'
			if (!Actor->Implements<UValGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);
					
					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);

					Ar.ArIsSaveGame = true;

					// Convert binary array back into actor's variables
					Actor->Serialize(Ar);

					IValGameplayInterface::Execute_OnActorLoaded(Actor);
					
					break;
				}
			}

		}
	}
	else
	{
		CurrentSaveGame = Cast<UValSaveGame>(UGameplayStatics::CreateSaveGameObject(UValSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
	
}
