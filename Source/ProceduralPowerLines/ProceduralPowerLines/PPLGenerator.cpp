// Fill out your copyright notice in the Description page of Project Settings.


#include "PPLGenerator.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "CableComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/StaticMeshSocket.h"

// Sets default values
APPLGenerator::APPLGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (Root)
	{
		SetRootComponent(Root);
	}

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	if (Spline)
	{
		Spline->SetupAttachment(Root);
	}
}

// Called when the game starts or when spawned
void APPLGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	ConnectCablesAndSockets();
}

void APPLGenerator::AddMeshesToSplinePoints()
{
	PlacedMeshes.Empty();

	if (!Spline)
	{
		return;
	}

	int SplinePoints = Spline->GetNumberOfSplinePoints();

	for (int i = 0; i < SplinePoints; i++)
	{
		FVector Location = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FRotator Rotation = Spline->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
		if (SplineMeshComponent)
		{
			SplineMeshComponent->SetStaticMesh(PlacedMesh);
			SplineMeshComponent->SetWorldLocation(Location);
			SplineMeshComponent->SetWorldRotation(Rotation);
			//Needed to change scale a little bit beacause of mesh itself
			SplineMeshComponent->SetWorldScale3D(FVector(0.2f, 1.f, 1.f));
			SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMeshComponent->RegisterComponent();
			
			PlacedMeshes.Add(PlacedMesh);
		}
	}
}

void APPLGenerator::ClearSplineChildComponents()
{
	//Remove existing meshes for Construction Script in the Editor
	TArray<USceneComponent*> ChildComponents = Spline->GetAttachChildren();
	for (USceneComponent* Child : ChildComponents)
	{
		if (Child && Child->IsA<UStaticMeshComponent>())
		{
			Child->DestroyComponent();
		}
	}
}

void APPLGenerator::ConnectCablesAndSockets()
{
	for (int i = 0; i < PlacedMeshes.Num(); i++)
	{
		if (PlacedMeshes.Num() > i + 1)
		{
			TArray<UStaticMeshSocket*> MeshSockets =  PlacedMeshes[i]->GetSocketsByTag(FString("Power"));
			TArray<UStaticMeshSocket*> NextMeshSockets =  PlacedMeshes[i + 1]->GetSocketsByTag(FString("Power"));
			
			for (int j = 0; j < MeshSockets.Num(); j++)
			{
				TObjectPtr<UCableComponent> CableComponent = NewObject<UCableComponent>(this, UCableComponent::StaticClass());
				CableComponent->RegisterComponent();

				//Set start location for cable
				FVector SplinePointLocation = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
				FRotator SplinePointRotation = Spline->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::Local);
				FVector WorldLocation = Spline->GetComponentTransform().TransformPosition(SplinePointLocation);
				FTransform PointTransform = UKismetMathLibrary::MakeTransform(WorldLocation, SplinePointRotation);
				FVector PointFinalLocation = UKismetMathLibrary::TransformLocation(PointTransform, MeshSockets[j]->RelativeLocation);
				CableComponent->SetRelativeLocation(PointFinalLocation);

				//Set end location for cable
				FVector SplineNextPointLocation = Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
				FRotator SplineNextPointRotation = Spline->GetRotationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
				FTransform NextPointTransform = UKismetMathLibrary::MakeTransform(SplineNextPointLocation, SplineNextPointRotation);
				FVector NextPointFinalLocation = UKismetMathLibrary::TransformLocation(NextPointTransform, NextMeshSockets[j]->RelativeLocation);
				CableComponent->EndLocation = NextPointFinalLocation;
				
				//Cable length
				float Length = UKismetMathLibrary::Vector_Distance(SplinePointLocation, SplineNextPointLocation);
				float CableLength = Length * CableLengthMultiplier;
				CableComponent->CableLength = CableLength;
				
				//Cable width
				CableComponent->CableWidth = CableWidth;
				CableComponent->SetVisibility(false);
				CableComponent->SetVisibility(true);
			}
		}
	}
}

// Called every frame
void APPLGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

