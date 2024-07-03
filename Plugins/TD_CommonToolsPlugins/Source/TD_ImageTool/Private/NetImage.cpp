#include "NetImage.h"
#include "HttpModule.h"
#include "IImageWrapperModule.h"
#include "ImageToolBPLibrary.h"

TSharedRef<SWidget> UNetImage::RebuildWidget()
{
	TryDownloadImage();
	return UImage::RebuildWidget();
}

void UNetImage::TryDownloadImage()
{
	if(ImageUrl.IsEmpty())
		return;
	FString CachePath = GetCacheFilePath();
	if (FPaths::FileExists(CachePath)) {
		TArray64<uint8> RawImageData;
		int Width = 0;
		int Heigth = 0;
		UImageToolBPLibrary::LoadImageToData(CachePath, RawImageData, Width, Heigth);
		FName ResourceName(*CachePath);
		DownloadedBrush = FSlateDynamicImageBrush::CreateWithImageData(ResourceName, FVector2D((float)Width, (float)Heigth), TArray<uint8>(RawImageData.GetData(), RawImageData.Num()));
		SetBrush(*DownloadedBrush);
	}
	else {
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->SetVerb(TEXT("GET"));
		HttpRequest->SetURL(ImageUrl);
		HttpRequest->SetHeader(TEXT("Accept"), TEXT("image/png, image/x-png, image/jpeg; q=0.8, image/vnd.microsoft.icon, image/x-icon, image/bmp, image/*; q=0.5, image/webp; q=0.0"));
		HttpRequest->OnProcessRequestComplete().BindUObject(this, &UNetImage::OnDownloadComplete);
		HttpRequest->ProcessRequest();
	}
}

void UNetImage::OnDownloadComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (!HttpResponse.IsValid() || HttpResponse->GetResponseCode() / 100 != 2){
		return;
	}
	if (HttpRequest->OnProcessRequestComplete().IsBound()){
		HttpRequest->OnProcessRequestComplete().Unbind();
	}
	const TArray<uint8>& Content = HttpResponse->GetContent();
	auto RequestUrl = HttpRequest->GetURL();
	TArray64<uint8> RawImageData;
	int Width = 0;
	int Heigth = 0;
	if (!UImageToolBPLibrary::GetWebpImageRawData(Content, RawImageData, Width, Heigth)){
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
		EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(Content.GetData(), Content.Num());
		if (ImageFormat == EImageFormat::Invalid)
		{
			FString ContentType = HttpResponse->GetContentType();
			UE_LOG(LogTemp, Error, TEXT("Image Download: Could not recognize file type of image downloaded from url %s, server-reported content type: %s"), *RequestUrl, *ContentType);
			return;
		}

		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
		if (!ImageWrapper.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Image Download: Unable to make image wrapper for image format %d"), (int32)ImageFormat);
			return;
		}
		if (!ImageWrapper->SetCompressed(Content.GetData(), Content.Num()))
		{
			UE_LOG(LogTemp, Error, TEXT("Image Download: Unable to parse image format %d from %s"), (int32)ImageFormat, *RequestUrl);
			return;
		}
		if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawImageData))
		{
			UE_LOG(LogTemp, Error, TEXT("Image Download: Unable to convert image format %d to BGRA 8"), (int32)ImageFormat);
			return;
		}
		Width = ImageWrapper->GetWidth();
		Heigth = ImageWrapper->GetHeight();
	}
	if (Width != 0 && Heigth != 0) {
		UImageToolBPLibrary::SaveImageFromRawData(RawImageData, GetCacheFilePath(), Width, Heigth);
		DownloadedBrush = FSlateDynamicImageBrush::CreateWithImageData(*RequestUrl, FVector2D((float)Width, (float)Heigth), TArray<uint8>(RawImageData.GetData(), RawImageData.Num()));
		if (DownloadedBrush) {
			SetBrush(*DownloadedBrush);
		}
	}
}

FString UNetImage::GetCacheFilePath()
{
	return FPaths::Combine(FPlatformProcess::UserSettingsDir(), FApp::GetProjectName(), "Thumbnail", FMD5::HashAnsiString(*ImageUrl) + ".png");
}

void UNetImage::SetImageUrl(FString InImageUrl)
{
	ImageUrl = InImageUrl;
	TryDownloadImage();
}
