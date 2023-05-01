// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OutlineHelperMisc.generated.h"


UENUM(BlueprintType, Category="OutlineHelperMisk")
enum class EOutlineDepth : uint8
{
	NoOutline = 0,
	Negative = 10,
	Positive = 11,
	Warning = 12
};

USTRUCT(BlueprintType, Category="OutlineHelperMisk")
struct FDepthParam
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName="MeshCustomDepth", Category="DepthParam")
	EOutlineDepth OutlineDepth = EOutlineDepth::NoOutline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName="OutlineColor", Category="DepthParam")
	FColor OutlineColor = FColor::Black; // Черный, потому что при считывании градиента,
										 //черные пиксели не будут учитываться
};


USTRUCT(BlueprintType, Category="OutlineHelperMisk")
struct MESHOUTLINEHELPER_API FOutlineSettings : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName="ImageSrc", Category="OutlineSettings")
	UTexture2D* GradientPalet = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName="MaterialSrc", Category="OutlineSettings")
	UMaterial* MaterialOutlinear = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName="DepthParams", Category="OutlineSettings")
	TArray<FDepthParam> CustomStencilDepthParams;
};



UCLASS(BlueprintType, Category="OutlineHelperMisk")
class UOutlineHelperMisc : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void ErrorLog(const FString &ClassName, const int& LineNum);
	static void InitializeDefaultPixelsPalette(TArray<FColor> & Pixels, const size_t Size = 256);
	static void UpdatePixels(TArray<FColor> &OldPixels, const TArray<FDepthParam> &NewPixels);
	static void UpdateTexture2D(UTexture2D* Texture, TArray<FColor> Pixels, int Size_X, int Size_Y, EPixelFormat PixelFormat = EPixelFormat::PF_B8G8R8A8);
};
