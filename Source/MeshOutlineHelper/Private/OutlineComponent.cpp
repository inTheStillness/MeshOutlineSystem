// Fill out your copyright notice in the Description page of Project Settings.


#include "OutlineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ActorComponent.h"


// Sets default values for this component's properties
UOutlineComponent::UOutlineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOutlineComponent::BeginPlay()
{
	Super::BeginPlay();
	GetStaticMeshOwner();

	// ...
	
}

void UOutlineComponent::GetStaticMeshOwner()
{
	if(const auto Owner = GetOwner())
	{
		TArray<UActorComponent*> candidates = Owner->GetComponentsByClass(UStaticMeshComponent::StaticClass());

		if(candidates.Num() < 1) {UOutlineHelperMisc::ErrorLog(__FILE__, __LINE__);return; }

		for(const auto candidate : candidates)
		{
			UStaticMeshComponent* component = Cast<UStaticMeshComponent>(candidate);
			// if(component == nullptr) {UOutlineHelperMisc::ErrorLog(__FILE__, __LINE__); continue;}
			if(component == nullptr) continue;
			component->SetRenderCustomDepth(true);
			m_StaticMeshOwner.Add(component);
		}
		
	}
	
}


// Called every frame
void UOutlineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UOutlineComponent::SwitchOutlineSettings(EOutlineDepth Depth)
{
	// if(m_StaticMeshOwner.Num() < 1 ) {UOutlineHelperMisc::ErrorLog(__FILE__, __LINE__);return;}
	if(m_StaticMeshOwner.Num() < 1 ) return;
	const uint8 iDepth = static_cast<uint8>(Depth);

	for(UStaticMeshComponent* Component : m_StaticMeshOwner)
	{
		if(Component == nullptr) {UOutlineHelperMisc::ErrorLog(__FILE__, __LINE__); continue;}
		Component->CustomDepthStencilValue = iDepth;
		Component->MarkRenderStateDirty();
	}
	
}

