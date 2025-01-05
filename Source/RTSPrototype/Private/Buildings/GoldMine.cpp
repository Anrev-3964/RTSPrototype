// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/GoldMine.h"

#include "Buildings/GoldMineConstructor.h"
#include "Components/BoxComponent.h"
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
	if (StaticMesh)
	{
		DefaultMesh = StaticMesh->GetStaticMesh();
	}
}

int AGoldMine::GetGoldAmount() const
{
	if (GoldAmount <= 0)
	{
		return 1000;
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
void AGoldMine::BuildMine(AActor*  OverlapActor)
{
	if (OverlapActor)
	{
		if (AGoldMineConstructor* GoldMineConstructor =   Cast<AGoldMineConstructor>(OverlapActor))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, "Inizia la costruzione della miniera");
			}
			GoldMineConstructor->OnBuildCompleteEvent.AddDynamic(this, &AGoldMine::MineCompleted);
			NewMesh = GoldMineConstructor->GetFinalMesh();
		}
	}
	OwnerPlayerState = GetOwnerPlayerState();
}

void AGoldMine:: MineCompleted(TEnumAsByte<EBuildState> BuildState)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, "Miniera finita");
	}
	if (NewMesh)
	{
		StaticMesh->SetStaticMesh(NewMesh);
	}
	//To DO : darela nuova mesh
	//To DO : cambiare la fazione
}
//NOTA : pe rora la quantita di oro estraibile non diminuisce a dogni estrazione
void AGoldMine::EstractGold(int GoldAmountToEstract)
{
	if (!OwnerPlayerState) return;
	if (GoldAmount >= 0)
	{
		OwnerPlayerState->AddGold(GoldAmountToEstract);
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
