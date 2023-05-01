// Fill out your copyright notice in the Description page of Project Settings.


#include "OutlineHelperMisc.h"
#include "CoreMinimal.h"
#include "Engine/Texture2D.h"

void UOutlineHelperMisc::ErrorLog(const FString& ClassName, const int& LineNum)
{
	UE_LOG(LogTemp, Error, TEXT("Error occured in class %ls on line %d"), *ClassName, LineNum);
}

void UOutlineHelperMisc::InitializeDefaultPixelsPalette(TArray<FColor>& Pixels, const size_t Size)
{
	for(size_t i = 0; i < Size; ++i)
		Pixels.Add(FColor::Black);
}

void UOutlineHelperMisc::UpdatePixels(TArray<FColor>& OldPixels, const TArray<FDepthParam>& NewPixels)
{
	for(const auto [OutlineDepth, OutlineColor] : NewPixels)
	{
		const uint8 index = static_cast<uint8>(OutlineDepth);
		if(index > OldPixels.Num()) continue; // Если CustomDepth за границами 256, то это бред)))
		OldPixels[index] = OutlineColor;
	}
}

void UOutlineHelperMisc::UpdateTexture2D(UTexture2D* Texture, TArray<FColor> Pixels, int Size_X, int Size_Y, EPixelFormat PixelFormat)
{
	constexpr uint8 PixelSize = sizeof(FColor);// Получаем размер одного пикселя. 
	const int PixelCount = Size_X*Size_Y; // Итоговый размер палитры с градиентом
	const size_t BufferSize = PixelCount*PixelSize; //Итоговый размер буфера всех пикселей изображения

	UE_LOG(LogTemp, Warning, TEXT("???"));
	
	if(PixelCount != Pixels.Num()) { ErrorLog(__FILE__, __LINE__); return;}
	if(Texture == nullptr) { ErrorLog(__FILE__, __LINE__); return;}
	
	FTexturePlatformData* PlatformData = Texture->GetPlatformData();
	
	if(PlatformData == nullptr) { ErrorLog(__FILE__, __LINE__); return;}
	
	
	PlatformData->PixelFormat = PixelFormat; // Задаем новый пиксель формат.
	//В нашем случае палитра корректно работает только с EPixelFormat::PF_B8G8R8A8
	PlatformData->SizeX = Size_X;
	PlatformData->SizeY = Size_Y;
	
	auto& BulkData = PlatformData->Mips[0].BulkData;
	void* Data = BulkData.Lock(LOCK_READ_WRITE); // Лочим работу с участком памяти, чтобы все не наебнулось.
	void* RawData = BulkData.Realloc(BufferSize); // Расширяем участок памяти, учитывая новый буфер
	FMemory::Memcpy(RawData, Pixels.GetData(), BufferSize); // Заполняем палитру новыми пикселями
	BulkData.Unlock();
	Texture->UpdateResource(); 
}
