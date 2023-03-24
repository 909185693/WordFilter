// Copyright 2022 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WordFilterBlueprintLibrary.generated.h"

/**
 * UWordFilterBlueprintLibrary
 */
UCLASS()
class WORDFILTER_API UWordFilterBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = WordFilter, meta = (DisplayName = "WordFilterLoadConfig"))
	static void LoadConfig(const FString& Directory);

	UFUNCTION(BlueprintCallable, Category = WordFilter, meta = (DisplayName = "WordFilterRelease"))
	static void Shutdown();

	UFUNCTION(BlueprintCallable, Category = WordFilter, meta = (DisplayName = "WordFilterCensor"))
	static FString Censor(const FString& Source);

	UFUNCTION(BlueprintCallable, Category = WordFilter, meta = (DisplayName = "WordFilterCheck"))
	static bool Check(const FString& Source);
};