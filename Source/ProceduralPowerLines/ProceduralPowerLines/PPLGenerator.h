// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPLGenerator.generated.h"

class USplineComponent;
class USceneComponent;
class USplineMeshComponent;

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components");
	//TObjectPtr<USplineComponent> SplineComponent;
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UStaticMesh> PlacedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TArray<TObjectPtr<UStaticMesh>> PlacedMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float CableLengthMultiplier = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float CableWidth = 5.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
