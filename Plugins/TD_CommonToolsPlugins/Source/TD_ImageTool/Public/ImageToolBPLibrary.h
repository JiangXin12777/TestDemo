// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IImageWrapper.h"
#include "ImageToolBPLibrary.generated.h"

UCLASS()
class IMAGETOOL_API UImageToolBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
UFUNCTION(BlueprintCallable, Category = "ImageTool")
	static void GetImageResolution(const FString& ImagePath, FVector2D& Resolution);

	UFUNCTION(BlueprintCallable, Category = "ImageTool")
	static void GetTextureResolution(UTexture2DDynamic* InTex, FVector2D& Resolution);

	UFUNCTION(BlueprintCallable, Category = "ImageTool")
	static UTexture2D* LoadImageToTexture2D(const FString& ImagePath);

	UFUNCTION(BlueprintCallable, Category = "ImageTool")
	static UTexture2DDynamic* LoadImageToTexture2DDy(const FString& ImagePath);

	UFUNCTION(BlueprintCallable, Category = "ImageTool")
	static bool SaveImageFromTexture2D(UTexture2D* InTex, const FString& SavePath);

	UFUNCTION(BlueprintCallable, Category = "ImageTool")
	static void SaveImageFromTexture2DDy(UTexture2DDynamic* InDyTex, const FString& SavePath);

	UFUNCTION(BlueprintCallable, Category = "ImageTool")
	static UTextureRenderTarget2D* RenderWidget(UWidget* Widget, const FVector2D& DrawSize);

	UFUNCTION(BlueprintCallable, Category = "ImageTool")
	static bool SaveRenderTarget2D(UTextureRenderTarget2D* RenderTarget2D, const FString& SavePath);

	UFUNCTION(BlueprintCallable, Category = "ImageTool")
	static UTexture2D* RenderWidgetToUTexture2D(UWidget* Widget, const FVector2D& DrawSize);

	UFUNCTION(BlueprintCallable, Category = "ImageTool")
	static bool PackTexture(const FString& TexturesDir, const FString& PackPath, const int PackSize = 2048);

	static bool GetWebpImageRawData(const TArray<uint8>& InWebpData, TArray64<uint8>& OutRawImageData, int& OutWidth, int& OutHeight);

	static bool LoadImageToData(const FString& ImagePath, TArray64<uint8>& OutData, int32& Width, int32& Height);

	static bool SaveImageFromRawData(TArray64<uint8>& RawData, const FString& SavePath, const int32& Width, const int32& Height);

	static void GetImageFormatFromPath(const FString& Path, EImageFormat& ImageFormat);
};
