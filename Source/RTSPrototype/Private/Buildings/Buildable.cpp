// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/Buildable.h"

#include "MaterialDomain.h"
#include "Components/BoxComponent.h"
#include "Framework/DataAssets/BuildItemDataAsset.h"
#include "Kismet/KismetMathLibrary.h"

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
}

void ABuildable::Init(UBuildItemDataAsset* BuildItemData, const TEnumAsByte<EBuildState> NewBuildState)
{
	if (!BuildItemData)
	{
		return;
	}

	BuildState = NewBuildState;
	BuildData = BuildItemData;

	if (BuildState == Building)
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
	DynamicOverlayMaterial->SetScalarParameterValue("Status", bCanPlace ? 1.0f : 0.0f);
}

void ABuildable::InitBuildPreview()
{
	if (!StaticMesh || !BuildData || !BoxCollider)
	{
		return;
	}

	if (UStaticMesh* PreviewMesh = BuildData->BuildingMeshComplete.LoadSynchronous())
	{
		StaticMesh->SetStaticMesh(PreviewMesh);
		UpdateCollider();
		SetOverlayMaterial();
	}
}

void ABuildable::StartBuild()
{
	BuildProgression = 0.0f;
	UpdateBuildProgressionMesh();
	GetWorldTimerManager().SetTimer(BuildTimer, this, &ABuildable::UpdateBuildProgression, 2.0f, true, 2.0f);

	BuildState = Building;
}

void ABuildable::EndBuild()
{
	GetWorldTimerManager().ClearTimer(BuildTimer);
	if (BuildState != BuildComplete)
	{
		BuildState = BuildAborted;
	}

	OnBuildCompleteEvent.Broadcast(BuildState);
}

void ABuildable::UpdateCollider()
{
	if (!StaticMesh && !BoxCollider)
	{
		return;
	}
	FVector MinMeshBounds, MaxMeshBounds;
	StaticMesh->GetLocalBounds(MinMeshBounds, MaxMeshBounds);

	BoxCollider->SetBoxExtent(FVector(
		                          FMath::Max((FMath::RoundToInt(MaxMeshBounds.X + 10.0f)), 100.0f),
		                          FMath::Max((FMath::RoundToInt(MaxMeshBounds.Y + 10.0f)), 100.0f),
		                          FMath::Max((FMath::RoundToInt(MaxMeshBounds.Z + 10.0f)), 100.0f)
	                          ), true);

	BoxCollider->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(FVector(1.0f, 0.0f, 0.0f)));
}

void ABuildable::SetOverlayMaterial()
{
	if (!BuildData)
	{
		return;
	}
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
	if (!BuildData || !StaticMesh)
	{
		return;
	}

	const int32 BuildMeshIndex = FMath::FloorToInt(BuildProgression * BuildData->BuildMeshes.Num());
	if (BuildData->BuildMeshes.IsValidIndex(BuildMeshIndex))
	{
		if (UStaticMesh* DisplayMesh = BuildData->BuildMeshes[BuildMeshIndex].LoadSynchronous())
		{
			StaticMesh->SetStaticMesh(DisplayMesh);
		}
	}
}

void ABuildable::UpdateBuildProgression()
{
	if (!StaticMesh)
	{
		return;
	}
	BuildProgression += 1.0f / BuildData->BuildMeshes.Num();
	if (BuildProgression > 1.0f)
	{
		if (UStaticMesh* DisplayMesh = BuildData->BuildingMeshComplete.LoadSynchronous())
		{
			StaticMesh->SetStaticMesh(DisplayMesh);
			StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			StaticMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		}
		BuildState = BuildComplete;
		EndBuild();
	}
	else
	{
		UpdateBuildProgressionMesh();
	}
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

// Called when the game starts or when spawned
void ABuildable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
