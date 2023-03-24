// Copyright 2022 CQUnreal. All Rights Reserved.

#include "WordFilterPrivate.h"
#include "WordFilter.h"

#define LOCTEXT_NAMESPACE "FWordFilterModule"

void FWordFilterModule::StartupModule()
{

}

void FWordFilterModule::ShutdownModule()
{
	WordFilter::Release();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWordFilterModule, WordFilter)