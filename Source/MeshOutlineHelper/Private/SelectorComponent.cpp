// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectorComponent.h"
#include "OutlineHelperMisc.h"
#include "OutlineComponent.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USelectorComponent::USelectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;



	// ...
}

void USelectorComponent::BeginPlay()
{
	Super::BeginPlay();

	MiscData.World = GetWorld();
	MiscData.Owner = Cast<APlayerController>(GetOwner());
	if(MiscData.World == nullptr || MiscData.Owner == nullptr) { UOutlineHelperMisc::ErrorLog(__FILE__, __LINE__); return;}
	MiscData.Camera = UGameplayStatics::GetPlayerCameraManager(MiscData.World, UGameplayStatics::GetPlayerControllerID(MiscData.Owner));
	if(m_RenderCamera == nullptr) FindRenderCamera();
	InitializeOutlineSettings();
}

void USelectorComponent::InitializeOutlineSettings() const
{
	if(OutlineSettings == nullptr) {UOutlineHelperMisc::ErrorLog(__FILE__, __LINE__); return;}
	
	TArray<FOutlineSettings*> Rows;
	OutlineSettings->GetAllRows<FOutlineSettings>(TEXT(""), Rows);

	for (const auto Row : Rows)
	{
		if(!Row) {UOutlineHelperMisc::ErrorLog(__FILE__, __LINE__); continue;}
		
		if(Row->GradientPalet == nullptr || Row->MaterialOutlinear == nullptr || Row->CustomStencilDepthParams.Num() < 1)
		{
			UOutlineHelperMisc::ErrorLog(__FILE__, __LINE__);
			continue;
		}

		TArray<FColor> Pixels;
		UOutlineHelperMisc::InitializeDefaultPixelsPalette(Pixels);
		UOutlineHelperMisc::UpdatePixels(Pixels, Row->CustomStencilDepthParams);
		UOutlineHelperMisc::UpdateTexture2D(Row->GradientPalet, Pixels, 32, 8);
		AddPostProcessMaterial(Row->MaterialOutlinear);
	}
}

void USelectorComponent::AddPostProcessMaterial(UMaterial* PostProcessMaterial) const
{
	if(m_RenderCamera == nullptr) {UOutlineHelperMisc::ErrorLog(__FILE__, __LINE__); return;}
	
	//m_RenderCamera->AddOrUpdateBlendable(PostProcessMaterial->GetMaterial(), 1.0f);
	m_RenderCamera->AddOrUpdateBlendable(PostProcessMaterial, 1.0f);
}

void USelectorComponent::FindRenderCamera()
{
	if(MiscData.Owner == nullptr) {UOutlineHelperMisc::ErrorLog(__FILE__, __LINE__); return;}

	AActor* CameraOwner = (m_CameraOwner == ERenderTarget::PlayerController)
	? Cast<AActor>(MiscData.Owner) : Cast<AActor>(MiscData.Owner->GetPawn());

	m_RenderCamera = FindObjectCamera(CameraOwner);
}

UCameraComponent* USelectorComponent::FindObjectCamera(AActor* Actor)
{
	if(Actor == nullptr) {UOutlineHelperMisc::ErrorLog(__FILE__, __LINE__); return nullptr;}

	return Actor->FindComponentByClass<UCameraComponent>();
}

UOutlineComponent* USelectorComponent::CompareObjectUnderCursor(const AActor* Candidate, bool & OutBool) const
{
	if(Candidate == nullptr)
	{
		OutBool = false;
		return nullptr;
	}
	UOutlineComponent* outliner = Cast<UOutlineComponent>(Candidate->GetComponentByClass(UOutlineComponent::StaticClass()));
	if(outliner == nullptr)
	{
		OutBool = false;
		return nullptr;
	}
	
	OutBool = true;
	return outliner;
}

void USelectorComponent::OutlineObjectUnderCursor()
{
	if(MiscData.Owner == nullptr) {UOutlineHelperMisc::ErrorLog(__FILE__, __LINE__); return;}

	FVector WorldLocation, WorldDirection;
	if (!MiscData.Owner->DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) { Deselect(); return; }

	const float TraceDistance = 10000.f;
	FHitResult HitResult;
	bool bHit = MiscData.World->LineTraceSingleByChannel(HitResult,
		WorldLocation, WorldLocation + WorldDirection * TraceDistance,
		ECollisionChannel::ECC_Visibility, FCollisionQueryParams::DefaultQueryParam);

	if(!bHit) { Deselect(); return;}
	AActor* HitActor = HitResult.GetActor();
	if(!HitActor) { Deselect(); return;}
	if(HitActor == m_ObjectUnderCursor) { return; }
	Deselect();
	Select(HitActor, EOutlineDepth::Positive);
	

}

void USelectorComponent::Select(AActor* Actor, EOutlineDepth Depth)
{
	if(Actor == nullptr) return;
	bool bHasComponent = false;
	UOutlineComponent* outliner = CompareObjectUnderCursor(Actor, bHasComponent);
	if(!bHasComponent) return;
	m_ObjectUnderCursor = Actor;
	outliner->SwitchOutlineSettings(Depth);
}

void USelectorComponent::Deselect()
{
	bool bHasComponent = false;
	UOutlineComponent* outliner = CompareObjectUnderCursor(m_ObjectUnderCursor, bHasComponent);
	if(!bHasComponent) return;
	outliner->SwitchOutlineSettings();
	m_ObjectUnderCursor = nullptr;
	
}


// Called every frame
void USelectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OutlineObjectUnderCursor();
}

