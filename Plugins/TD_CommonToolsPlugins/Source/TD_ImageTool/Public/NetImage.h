#pragma once

#include "Components/Image.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "NetImage.generated.h"

UCLASS(meta = (DisplayName = "NetImage"))					
class IMAGETOOL_API UNetImage : public UImage {
	GENERATED_BODY()
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void TryDownloadImage();

	void OnDownloadComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	FString GetCacheFilePath();

	UFUNCTION(BlueprintCallable)
	void SetImageUrl(FString InImageUrl);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ImageUrl;

	TSharedPtr<FSlateDynamicImageBrush> DownloadedBrush;
};