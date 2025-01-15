// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/GoldMine.h"

#include "Buildings/GoldMineConstructor.h"
#include "Components/BoxComponent.h"
#include "Framework/DataAssets/BuildItemDataAsset.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"


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
	Super::BeginPlay();
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
			/**
			TSoftObjectPtr<UStaticMesh> NewMesh = BuildItemDataAsset->BuildingMeshComplete;
			CompletedMineMesh = NewMesh;
			Buildable->OnBuildCompleteEvent.AddDynamic(this, &AGoldMine::MineCompleted);
			Buildable->SetActorLocation(GetActorLocation());
			**/
			Buildable->OnBuildCompleteEvent.AddDynamic(this, &AGoldMine::MineCompleted);
			Buildable->SetActorLocation(GetActorLocation());
			BuildingActorCompleteClass =  BuildItemDataAsset->BuildingActorComplete;
		}

		NewOwnerFaction = Buildable->GetFaction();
	}
}

void AGoldMine:: MineCompleted(const TEnumAsByte<EBuildState> BuildState)
{
	FTimerHandle TimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGoldMine::SetStaticMeshFromActor, 0.5f, false);
	MineCurrentLevel++;
	SetStaticMeshFromActor();
	CurrentFaction = NewOwnerFaction;
	OwnerPlayerState = GetOwnerPlayerState();

	/**
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
	**/
}
//NOTA : pe rora la quantita di oro estraibile non diminuisce a dogni estrazione
void AGoldMine::EstractGold()
{
	if (GoldMineData)
	{
		//Set GoldAmountEstract Based On current level
		switch (MineCurrentLevel)
		{
			case 1:
			GoldEstractAmount = GoldMineData->TierOneGoldEstractionAmount;
			break;
			case 2:
				GoldEstractAmount = GoldMineData->TierTwoGoldEstractionAmount;
			break;
			case 3:
				GoldEstractAmount = GoldMineData->TierThreeGoldEstractionAmount;
			break;

			default:
				GoldEstractAmount = 1;
			break;
		}
		
		if (!OwnerPlayerState) return;

		if (GoldEstractAmount >= GoldAmount)
		{
			OwnerPlayerState->AddGold(GoldAmount);
			GoldAmount = 0;
			Destroy();
		}
		else
		{
			if (GoldMineData && GoldMineData->GoldCollectedAudioClip)
			{
				UGameplayStatics::PlaySoundAtLocation(this, GoldMineData->GoldCollectedAudioClip, GetActorLocation());
			}
			GoldAmount -= GoldEstractAmount;
			OwnerPlayerState->AddGold(GoldEstractAmount);
		}
	}
}

int AGoldMine::GetGoldEstractionAmount()
{
	if (GoldEstractAmount >0)
	{
		return GoldEstractAmount;
	}
	return 1;
}

void AGoldMine::SetStaticMeshFromActor()
{
	if (!BuildingActorCompleteClass) return;

	// load Actor from soft pointer
	//instantiate Actor
	AActor* BuildingActorComplete = GetWorld()->SpawnActor<AActor>(BuildingActorCompleteClass);

	if (!BuildingActorComplete) return;

	//Call an event form blueprint

	FName EventName("SetTierMeshes");

	//Find The vent to call
	if (UFunction* EventFunction = BuildingActorComplete->FindFunction(EventName))
	{

		struct FCustomEventParams
		{
			int NewTier = 1; 
		};
		
		FCustomEventParams Params;
		Params.NewTier = MineCurrentLevel;
		
		BuildingActorComplete->ProcessEvent(EventFunction, &Params);
	}
	
	TArray<UStaticMeshComponent*> PreviewComponents;
	GetComponents<UStaticMeshComponent>(PreviewComponents);
	
	for (UStaticMeshComponent* Comp : PreviewComponents)
	{
		if (Comp)
		{
			Comp->DestroyComponent();
		}
	}

	// Get all  UStaticMeshComponent from BuildingActorComplete
	TArray<UStaticMeshComponent*> MeshComponents;
	BuildingActorComplete->GetComponents<UStaticMeshComponent>(MeshComponents);
	
	for (UStaticMeshComponent* MeshComp : MeshComponents)
	{
		if (MeshComp && MeshComp->GetStaticMesh())
		{
			// For every UStaticMeshComponent in ABuildingActorComplete, create a new mesh 
			UStaticMeshComponent* NewPreviewComp = NewObject<UStaticMeshComponent>(this);
			if (NewPreviewComp)
			{
				NewPreviewComp->SetStaticMesh(MeshComp->GetStaticMesh());
				NewPreviewComp->SetRelativeTransform(MeshComp->GetRelativeTransform());
				FVector NewScale(0.5f, 0.5f, 0.5f); // Scala su tutti gli assi X, Y e Z
				NewPreviewComp->SetRelativeScale3D(NewScale);
				NewPreviewComp->AttachToComponent(BoxCollider, FAttachmentTransformRules::KeepRelativeTransform);

				NewPreviewComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
				
				NewPreviewComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
				
				NewPreviewComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
				
				NewPreviewComp->RegisterComponent();
			}
		}
	}
	BuildingActorComplete->Destroy();
}

void AGoldMine::Select()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Miniera selezionata (c++)");
	}
	OnMineSelect.Broadcast();
}

void AGoldMine::DeSelect()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Miniera deselezionata (c++)");
	}
	OnMineSelect.Broadcast();
}

void AGoldMine::Highlight(const bool Highlight)
{
}

bool AGoldMine::ActorIsATriggerArea()
{
	return true;
}
