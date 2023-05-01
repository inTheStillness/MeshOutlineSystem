// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SelectorComponent.generated.h"

class UCameraComponent;
class UDataTable;
class UOutlineComponent;
class APlayerCameraManager;

USTRUCT()
struct FMiscData
{
	GENERATED_BODY()

public:
	UWorld* World = nullptr;
	APlayerController* Owner = nullptr;
	APlayerCameraManager* Camera = nullptr;
	
};

UENUM(BlueprintType, Category="SelectorComponent")
enum class ERenderTarget : uint8
{
	PlayerPawn = 0,
	PlayerController = 1
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MESHOUTLINEHELPER_API USelectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SelectorComponent")
	UDataTable* OutlineSettings = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SelectorComponent")
	UCameraComponent* m_RenderCamera = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SelectorComponent")
	ERenderTarget m_CameraOwner = ERenderTarget::PlayerPawn;

protected:
	FMiscData MiscData;
	AActor* m_ObjectUnderCursor = nullptr;
	
public:	
	// Sets default values for this component's properties
	USelectorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void InitializeOutlineSettings() const;
	void AddPostProcessMaterial(UMaterial* PostProcessMaterial) const;
	void FindRenderCamera();
	
	static UCameraComponent* FindObjectCamera(AActor* Actor);



protected:
	UOutlineComponent* CompareObjectUnderCursor(const AActor* Candidate, bool& OutBool) const;
	void OutlineObjectUnderCursor();
	void Select(AActor* Actor, EOutlineDepth Depth);
	void Deselect();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
