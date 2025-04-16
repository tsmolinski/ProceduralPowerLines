// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPLGenerator.generated.h"

class USplineComponent;
class USceneComponent;
class UHierarchicalInstancedStaticMeshComponent;

UCLASS()
class PROCEDURALPOWERLINES_API APPLGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPLGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddMeshesToSplinePoints();

	UFUNCTION(BlueprintCallable)
	void ClearSplineChildComponents();
	
	void ConnectCablesAndSockets();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UStaticMesh> PlacedMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	TArray<TObjectPtr<UStaticMesh>> PlacedMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float CableLengthMultiplier = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float CableWidth = 5.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
