// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/Buildable.h"

#include "MaterialDomain.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Framework/RTSPlayerState.h"
#include "Framework/DataAssets/BuildItemDataAsset.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABuildable::ABuildable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	RootComponent = BoxCollider;
	BoxCollider->SetCollisionProfileName("OverlapAll");
	BoxCollider->SetGenerateOverlapEvents(true);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName("OverlapAll");

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	BuildingCurrentHealth = 1.0f;
}

void ABuildable::Init(UBuildItemDataAsset* BuildItemData, const TEnumAsByte<EBuildState> NewBuildState)
{
	if (!BuildItemData) return;

	BuildState = NewBuildState;
	BuildData = BuildItemData;
	BuildID = BuildData->BuildID;
	UE_LOG(LogTemp, Error, TEXT("BuildID: %u"), BuildID);

	if (BuildState == EBuildState::Building)
	{
		StartBuild();
	}
	else
	{
		InitBuildPreview();
	}
}

void ABuildable::UpdateOverlayMaterial(const bool bCanPlace) const
{
	DynamicOverlayMaterial->SetScalarParameterValue("Status", bCanPlace? 1.0f:0.0f);
}

void ABuildable::InitBuildPreview()
{
	if (!BuildData || !BoxCollider) return;

	// load Actor from soft pointer
	TSubclassOf<AActor> BuildingActorClass = BuildData->BuildingActorComplete;
	if (!BuildingActorClass) return;

	//instantiate Actor
	AActor* BuildingActorComplete = GetWorld()->SpawnActor<AActor>(BuildingActorClass);
	if (!BuildingActorComplete) return;

	// remove already existing meshComponents
	TArray<UStaticMeshComponent*> PreviewComponents;
	GetComponents<UStaticMeshComponent>(PreviewComponents);

	for (UStaticMeshComponent* Comp : PreviewComponents)
	{
		if (Comp && Comp != StaticMesh)
		{
			Comp->DestroyComponent();
		}
	}

	// Get al  UStaticMeshComponent from BuildingActorComplete
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
				NewPreviewComp->AttachToComponent(BoxCollider, FAttachmentTransformRules::KeepRelativeTransform);

				NewPreviewComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				
				NewPreviewComp->RegisterComponent();
			}
		}
	}

	// Aggiorna il collider e il materiale di overlay
	BuildingActorComplete->Destroy();
	UpdateCollider();
	SetOverlayMaterial();
	
	/**
	if (UStaticMesh* PreviewMesh = BuildData->BuildingMeshComplete.LoadSynchronous())
	{
		StaticMesh->SetStaticMesh(PreviewMesh);
		UpdateCollider();
		SetOverlayMaterial();
	}
	**/
}

void ABuildable::StartBuild()
{
	BuildProgression = 0.0f;
	UpdateBuildProgressionMesh();
	GetWorldTimerManager().SetTimer(BuildTimer, this, &ABuildable::UpdateBuildProgression, 2.0f, true, 2.0f);

	BuildState = EBuildState::Building;

	OnBuildStarted.Broadcast(this);

	if (BuildData)
	{
		//Assign Building Stats from DataAsset
		BuildingCurrentHealth = BuildData->BuildInProgressHP;
		//Spawn Niagara System
		if (BuildData->BuildingInProgressNiagaraSystem)
		{
			//Spawn Niagara Effect
			FVector Location = GetActorLocation() + BuildData->BuildingInProgressNiagara_SpawnOffset;
			FRotator Rotation = FRotator::ZeroRotator;

			SpawnedBuildingNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),         // World
				BuildData->BuildingInProgressNiagaraSystem,      // Niagara System To Spawn
				Location,           // Position
				Rotation,           // Rotation
				FVector(1.0f)); // Scale
		}

		if (SpawnedBuildingNiagaraComponent)
		{
			// Imposta il parametro utente "LoopDuration" su 5.0f
			float LoopDuration = BuildData->BuildingInProgressNiagara_LoopDuration;
			SpawnedBuildingNiagaraComponent->SetVariableFloat(FName("LoopDuration"), LoopDuration);
		}

		//PlaySound At Location
		if (BuildData->BuildingInProgressAudioClip && AudioComponent)
		{
			AudioComponent->SetSound(BuildData->BuildingInProgressAudioClip);
			AudioComponent->Play();
		}
	}
}

void ABuildable::EndBuild()
{
	CurrentFaction = EFaction::Team1;
	if (AudioComponent)
	{
		AudioComponent->Stop();
	}
	if (BuildData)
	{
		//Build complete Niagara Compoent 
		if (BuildData->BuildingCompletedNiagaraSystem)
		{
			BuildingCurrentHealth = BuildData->HP;
			//Spawn NiagaraSystem
			if (BuildData->BuildingCompletedNiagaraSystem)
			{
				FVector Location = GetActorLocation() + BuildData->BuildingCompletedNiagara_SpawnOffset;
				FRotator Rotation = FRotator::ZeroRotator;

				SpawnedBuildingNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),         // World
					BuildData->BuildingCompletedNiagaraSystem,      // Niagara System To Spawn
					Location,           // Position
					Rotation,           // Rotation
					FVector(1.0f)); // Scale
			}
			//Set Niagara Loop Duration
			if (SpawnedBuildingNiagaraComponent)
			{
				float LoopDuration = BuildData->BuildingCompletedNiagara_LoopDuration;
				SpawnedBuildingNiagaraComponent->SetVariableFloat(FName("Lifetime"), LoopDuration);
			}
		}

		if (BuildData->CanBeBuiltCloseToGoldSources)
		{
			//Buildable get destroyed if built clos eto Gold Source, this play the sound on it's own in way to work after BuildableDestroy
			UGameplayStatics::PlaySoundAtLocation(this, BuildData->BuildingCompletedAudioClip, GetActorLocation());
		}
		else
		{
			//PlaySound At Location
			if (BuildData->BuildingCompletedAudioClip && AudioComponent)
			{
				AudioComponent->SetSound(BuildData->BuildingCompletedAudioClip);
				AudioComponent->Play();
			}
		}
	}
	
	GetWorldTimerManager().ClearTimer(BuildTimer);
	if (BuildState != EBuildState::BuildComplete)
	{
		BuildState = EBuildState::BuildAborted;
		bBuildingConstructed = true;
	}

	OnBuildCompleteEvent.Broadcast(BuildState);
}

void ABuildable::UpdateCollider()
{
}

void ABuildable::SetOverlayMaterial()
{
	if (!BuildData) return;
	const FSoftObjectPath AssetPath = BuildData->PlaceMaterial.ToSoftObjectPath();
	if (UMaterialInstance* OverlayMaterial = Cast<UMaterialInstance>(AssetPath.TryLoad()))
	{
		DynamicOverlayMaterial = UMaterialInstanceDynamic::Create(OverlayMaterial, this);
		if (DynamicOverlayMaterial)
		{
			TArray<UStaticMeshComponent*> MeshComponents;
			GetComponents<UStaticMeshComponent>(MeshComponents);
			for (int32 i = 0; i < MeshComponents.Num(); i++)
			{
				MeshComponents[i]->SetOverlayMaterial(DynamicOverlayMaterial);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FAILED INSIDE IF() DYNAMIC OVERLAY MATERIAL"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FAILED DYNAMIC OVERLAY MATERIAL"));
	}
}

void ABuildable::UpdateBuildProgressionMesh()
{
	if (!BuildData || !StaticMesh) return;

	const int32 BuildMeshIndex = FMath::FloorToInt(BuildProgression * BuildData->BuildMeshes.Num());
	if (BuildData->BuildMeshes.IsValidIndex(BuildMeshIndex))
	{
		if (UStaticMesh* DisplayMesh = BuildData->BuildMeshes[BuildMeshIndex].LoadSynchronous())
		{
			StaticMesh->SetStaticMesh(DisplayMesh);
			
			StaticMesh->SetRelativeScale3D(BuildData->DesiredScale);
		}
	}
}

void ABuildable::UpdateBuildProgression()
{
	if (!StaticMesh) return;
	
	BuildProgression += 1.0f / BuildData->BuildMeshes.Num();
	if (BuildProgression > 1.0f)
	{
		if (TSubclassOf<AActor> BuildingActorClass = BuildData->BuildingActorComplete)
		{
			SetStaticMeshFromActor();
		}
		BuildState = EBuildState::BuildComplete;
		EndBuild();
	}
	else
	{
		UpdateBuildProgressionMesh();
	}
}

void ABuildable::SetStaticMeshFromActor()
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
		Params.NewTier = 0;
		
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
//Get The Player State of it'sown Faction
ARTSPlayerState* ABuildable::GetOwnerPlayerState() const
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

UBuildItemDataAsset* ABuildable::GetBuildData() const
{
	return BuildData;
}

UMaterialInstance* ABuildable::GetHighlightMaterial() const
{
	if (const UBuildItemDataAsset* BuildItemData = GetBuildItemData())
	{
		const FSoftObjectPath AssetPath = BuildItemData->HighlightMaterial.ToSoftObjectPath();
		return Cast<UMaterialInstance>(AssetPath.TryLoad());
	}

	return nullptr;
}


int ABuildable::GetBuildID() const
{
	return BuildID;
}

void ABuildable::InflictDamage(const float AttackValue)
{
	BuildingCurrentHealth -= AttackValue;
	BuildingCurrentHealth = FMath::Clamp(BuildingCurrentHealth, 0.f, BuildingCurrentHealth);

	//call OnDamageTakenEvent if there is at least 1 subscriber (There is some extra logic in blueprint)
	if (OnDamageTaken.IsBound())
	{
		OnDamageTaken.Broadcast();
	}
	//Health =< 0 :  target death
	if (BuildingCurrentHealth <= 0.f)
	{
		OnBuildDestroyed.Broadcast();
		Destroy();
	}
}

EFaction ABuildable::GetFaction() const
{
	return CurrentFaction;
}

void ABuildable::Select()
{
	bSelected = true;
	UE_LOG(LogTemp, Warning, TEXT("BUILDING SELECTED"));
	Highlight(bSelected);
	OnMyEventTriggered.Broadcast();
}

void ABuildable::DeSelect()
{
	bSelected = false;
	Highlight(bSelected);
	OnMyEventTriggered.Broadcast();
}

void ABuildable::Highlight(const bool Highlight)
{
	 TArray<UStaticMeshComponent*> StaticMeshComponents;
    GetComponents<UStaticMeshComponent>(StaticMeshComponents);

    for (UStaticMeshComponent* MeshComponent : StaticMeshComponents)
    {
        if (Highlight)
        {
            if (UMaterialInstance* HighlightMaterial = GetHighlightMaterial())
            {
                if (HighlightMaterial->GetMaterial()->MaterialDomain == MD_PostProcess)
                {
                    UE_LOG(LogTemp, Log, TEXT("Applying Custom Depth for post-process material"));

                    TArray<UPrimitiveComponent*> PrimitiveComponents;
                    GetComponents<UPrimitiveComponent>(PrimitiveComponents);

                    for (UPrimitiveComponent* VisualComponent : PrimitiveComponents)
                    {
                        if (VisualComponent)
                        {
                            VisualComponent->SetRenderCustomDepth(true);
                            UE_LOG(LogTemp, Log, TEXT("Enabled Custom Depth for %s"), *VisualComponent->GetName());
                        }
                    }
                }
                else
                {
                    MeshComponent->SetOverlayMaterial(HighlightMaterial);
                    UE_LOG(LogTemp, Log, TEXT("Applied Overlay Material: %s to %s"), 
                        *HighlightMaterial->GetName(), *MeshComponent->GetName());
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to get Highlight Material. Falling back."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Disabling highlight"));

            // Disable overlay material
            MeshComponent->SetOverlayMaterial(nullptr);
            UE_LOG(LogTemp, Log, TEXT("Removed Overlay Material from %s"), *MeshComponent->GetName());

            // Disable custom depth
            TArray<UPrimitiveComponent*> PrimitiveComponents;
            GetComponents<UPrimitiveComponent>(PrimitiveComponents);

            for (UPrimitiveComponent* VisualComponent : PrimitiveComponents)
            {
                if (VisualComponent)
                {
                    VisualComponent->SetRenderCustomDepth(false);
                    UE_LOG(LogTemp, Log, TEXT("Disabled Custom Depth for %s"), *VisualComponent->GetName());
                }
            }
        }
    }
}

void ABuildable::AttackSelectable(const float DamageAmount)
{
	InflictDamage(DamageAmount);
}


bool ABuildable::GetBuildingConstructed()
{
	return bBuildingConstructed;
}
// Called when the game starts or when spawned
void ABuildable::BeginPlay()
{
	Super::BeginPlay();
	OwnerPlayerState = GetOwnerPlayerState();
}

// Called every frame
void ABuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AActor* ABuildable::GetActor()
{
	if (WeakActorPtr.IsValid())
	{
		return WeakActorPtr.Get();
	}
	return nullptr;
}

void ABuildable::SetCurrentFaction(EFaction NewFaction)
{
	if (CurrentFaction != NewFaction)
	{
		CurrentFaction = NewFaction;
	}
}


