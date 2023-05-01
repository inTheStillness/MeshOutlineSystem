// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OutlineHelperMisc.h"
#include "OutlineComponent.generated.h"

class UStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MESHOUTLINEHELPER_API UOutlineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOutlineComponent();

private:
	TArray<UStaticMeshComponent*> m_StaticMeshOwner;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void GetStaticMeshOwner();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SwitchOutlineSettings(EOutlineDepth Depth = EOutlineDepth::NoOutline);

		
};
