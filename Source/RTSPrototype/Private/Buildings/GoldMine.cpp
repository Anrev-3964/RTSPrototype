// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/GoldMine.h"

#include "Buildings/GoldMineConstructor.h"
#include "Components/BoxComponent.h"
#include "Framework/DataAssets/BuildItemDataAsset.h"
#include "GameFramework/GameStateBase.h"


AGoldMine::AGoldMine(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CurrentFaction = EFaction::Team2;
	GoldAmount = 9999;
	CurrentGoldAmount = GoldAmount;

	RootComponentIntermediate = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponentIntermediate"));
	RootComponent = RootComponentIntermediate;
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(RootComponent);
	BoxCollider->SetCollisionProfileName("OverlapAll");
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName("OverlapAll");
}

void AGoldMine::BeginPlay()
{
	SetMineFromDataAsset();
}

void AGoldMine::SetMineFromDataAsset()
{
	if (!GoldMineData) return;

	GoldAmount = GoldMineData->GetMineGoldAmount();
	if (StaticMesh)
	{
		if (UStaticMesh* Mesh = GoldMineData->MeshReference.LoadSynchronous())
		{
			StaticMesh->SetStaticMesh(Mesh);
		}
	}

	
}

int AGoldMine::GetGoldAmount() const
{
	if (GoldAmount <= 0)
	{
		return 0;
	}
	return GoldAmount;
}

int AGoldMine::GetCurrentGoldAmount() const 
{
	return CurrentGoldAmount;
}

EFaction AGoldMine::GetFaction() const
{
	return CurrentFaction;
}

//Get The Player State of it'sown Faction
ARTSPlayerState* AGoldMine::GetOwnerPlayerState() const
{
	if (AGameStateBase* GameState = GetWorld()->GetGameState())
	{
		for (APlayerState* PlayerState : GameState->PlayerArray)
		{
			if (ARTSPlayerState* MyPlayerState = Cast<ARTSPlayerState>(PlayerState))
			{
				if (IFactionsUtils* StateFaction = Cast<IFactionsUtils>(MyPlayerState))
				{
					if (StateFaction->GetFaction() == CurrentFaction)
					{
						return MyPlayerState;
					}
				}
			}
		}
	}
	return nullptr;
}


void AGoldMine::SetCurrentFaction(EFaction NewFaction)
{
	if (CurrentFaction != NewFaction)
	{
		CurrentFaction = NewFaction;
	}
}

//Called in bueprint 
void AGoldMine::PreBuildMine(AActor*  OverlapActor)
{
	if (OverlapActor)
	{
		if (AGoldMineConstructor* GoldMineConstructor =   Cast<AGoldMineConstructor>(OverlapActor)) //MUST DERIVE FROM BUILDABLE
		{
			GoldMineConstructor->OnBuildStarted.AddDynamic(this, &AGoldMine::MineStarted);
		}
	}
}

void AGoldMine::MineStarted(ABuildable* Buildable)
{
	if (!Buildable) return;
	{
		
		if (UBuildItemDataAsset* BuildItemDataAsset = Buildable->GetBuildItemData())
		{
			TSoftObjectPtr<UStaticMesh> NewMesh = BuildItemDataAsset->BuildingMeshComplete;
			CompletedMineMesh = NewMesh;
			Buildable->OnBuildCompleteEvent.AddDynamic(this, &AGoldMine::MineCompleted);
			Buildable->SetActorLocation(GetActorLocation());
		}

		NewOwnerFaction = Buildable->GetFaction();
	}
}

void AGoldMine:: MineCompleted(const TEnumAsByte<EBuildState> BuildState)
{
	CurrentFaction = NewOwnerFaction;
	
	if (StaticMesh && CompletedMineMesh.IsValid())
	{
		UStaticMesh* LoadedMesh = CompletedMineMesh.Get();

		if (!LoadedMesh)
		{
			LoadedMesh = CompletedMineMesh.LoadSynchronous();
		}
		
		if (LoadedMesh)
		{
			StaticMesh->SetStaticMesh(LoadedMesh);
		}
	}

	OwnerPlayerState = GetOwnerPlayerState();
	
}
//NOTA : pe rora la quantita di oro estraibile non diminuisce a dogni estrazione
void AGoldMine::EstractGold()
{
	int Amount = 5;
	if (!OwnerPlayerState) return;

	if (Amount >= GoldAmount)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red , "Oro finito");
		}
		OwnerPlayerState->AddGold(GoldAmount);
		GoldAmount = 0;
		Destroy();
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "c'e ancora oro");
		}
		GoldAmount -= Amount;
		OwnerPlayerState->AddGold(Amount);
	}
}

void AGoldMine::Select()
{
}

void AGoldMine::DeSelect()
{
}

void AGoldMine::Highlight(const bool Highlight)
{
}

bool AGoldMine::ActorIsATriggerArea()
{
	return true;
}
