// Copyright 2022 CQUnreal. All Rights Reserved.

#include "WordFilterBlueprintLibrary.h"
#include "WordFilter.h"


void UWordFilterBlueprintLibrary::LoadConfig(const FString& Directory)
{
	WordFilter::Get().LoadConfig(*Directory);
}

void UWordFilterBlueprintLibrary::Shutdown()
{
	WordFilter::Release();
}

FString UWordFilterBlueprintLibrary::Censor(const FString& Source)
{
	FString Result = Source;

	WordFilter::Get().Censor(Result);

	return Result;
}

bool UWordFilterBlueprintLibrary::Check(const FString& Source)
{
	return WordFilter::Get().Check(Source);
}