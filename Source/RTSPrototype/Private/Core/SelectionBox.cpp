// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SelectionBox.h"

#include "Components/DecalComponent.h"
#include "Core/Selectable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASelectionBox::ASelectionBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetBoxExtent(FVector(1.0f, 1.0f, 1.0f));

	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = BoxCollider;
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASelectionBox::OnBoxBeginOverlap);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	DecalComponent->SetupAttachment(RootComponent);

	
	
	bBoxSelected = false;
	
}

void ASelectionBox::Start(FVector Position, const FRotator Rotation)
{
	if (!DecalComponent) return;

	StartLocation = FVector(Position.X, Position.Y, 0);
	StartRotation = FRotator(0, Rotation.Yaw, 0);

	SetActorLocation(StartLocation);
	SetActorRotation(StartRotation);
	SetActorEnableCollision(true);
	
	DecalComponent->SetVisibility(true);
	InBox.Empty();
	CenterInBox.Empty();
	bBoxSelected = true;
}

void ASelectionBox::End()
{
	if (!SPlayer) return;
	bBoxSelected = false;
	SetActorEnableCollision(false);
	DecalComponent->SetVisibility(false);

	if (CenterInBox.Num() == 0)
	{
		SPlayer->HandleSelection(nullptr);
	}
	else
	{
		SPlayer->HandleSelection(CenterInBox);
	}

	InBox.Empty();
	CenterInBox.Empty();
}

// Called when the game starts or when spawned
void ASelectionBox::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorEnableCollision(false);
	if (DecalComponent)
	{
		DecalComponent->SetVisibility(false);
	}

	SPlayer = Cast<ASPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (SPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay: SPlayer successfully initialized"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BeginPlay: SPlayer is nullptr!"));
	}
}

void ASelectionBox::Adjust() const
{
	if (!SPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("ASelectionBox::Adjust: SPlayer is nullptr"));
		return;
	}

	if (!BoxCollider)
	{
		UE_LOG(LogTemp, Error, TEXT("ASelectionBox::Adjust: BoxCollider is nullptr"));
		return;
	}

	if (!DecalComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ASelectionBox::Adjust: DecalComponent is nullptr"));
		return;
	}

	const FVector CurrentMouseLocOnTerrain = SPlayer->GetMousePositionOnTerrain();
	const FVector EndPoint = FVector(CurrentMouseLocOnTerrain.X, CurrentMouseLocOnTerrain.Y, 0);
	BoxCollider->SetWorldLocation(UKismetMathLibrary::VLerp(StartLocation, EndPoint, 0.5f));

	FVector NewExtent = FVector(GetActorLocation().X, GetActorLocation().Y, 0) - EndPoint;
	NewExtent = GetActorRotation().GetInverse().RotateVector(NewExtent);
	NewExtent = NewExtent.GetAbs();
	NewExtent.Z += 100000.0f;
	BoxCollider->SetBoxExtent(NewExtent);

	FVector DecalSize;
	DecalSize.X = NewExtent.Z;
	DecalSize.Y = NewExtent.Y;
	DecalSize.Z = NewExtent.X;
	DecalComponent->DecalSize = DecalSize;
}

void ASelectionBox::ManageUnits()
{
	if (!BoxCollider) return;

	for (int i= 0; i < InBox.Num(); i++)
	{
		if (!InBox[i]) // Null check for safety
		{
			UE_LOG(LogTemp, Warning, TEXT("ManageUnits: Null Actor in InBox at index %d"), i);
			continue;
		}
		
		FVector ActorCenter = InBox[i]->GetActorLocation();
		const FVector LocalActorCenter = BoxCollider->GetComponentTransform().InverseTransformPosition(ActorCenter);

		const FVector LocalExtents = BoxCollider->GetUnscaledBoxExtent();
		if (LocalActorCenter.X >= -LocalExtents.X && LocalActorCenter.X <= LocalExtents.X
			&& LocalActorCenter.Y >= -LocalExtents.Y && LocalActorCenter.Y <= LocalExtents.Y
			&& LocalActorCenter.Z >= -LocalExtents.Z && LocalActorCenter.Z <= LocalExtents.Z)
		{
			if (!CenterInBox.Contains(InBox[i]))
			{
				if (CenterInBox.Num() < UnitsLimit)
				{
					CenterInBox.Add(InBox[i]);
					HandleHighlight(InBox[i], true);
				}
			}
		}
		else
		{
			CenterInBox.Remove(InBox[i]);
			HandleHighlight(InBox[i], false);
		}
	}
}

void ASelectionBox::HandleHighlight(AActor* ActorInBox, const bool Highlight) const
{
	if (ISelectable* Selectable = Cast<ISelectable>(ActorInBox))
	{
		UE_LOG(LogTemp, Log, TEXT("HandleHighlight: Highlighting Actor %s with value %s"), 
			*ActorInBox->GetName(), Highlight ? TEXT("true") : TEXT("false"));
        
		Selectable->Highlight(Highlight); // Ensure this function is implemented correctly
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleHighlight: Actor %s does not implement ISelectable"), *ActorInBox->GetName());
	}
}

void ASelectionBox::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	//check is a selectable unit
	if (ISelectable* Selectable = Cast<ISelectable>(OtherActor))
	{
		if (InBox.Num() < UnitsLimit)
		{
			InBox.AddUnique(OtherActor);
			UE_LOG(LogTemp, Warning, TEXT("OVERLAPPED WITH:%p "), OtherActor);
		}
	}
}

// Called every frame
void ASelectionBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bBoxSelected)
	{
		Adjust();
		ManageUnits();
	}
}

