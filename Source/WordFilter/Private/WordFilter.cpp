// Copyright 2022 CQUnreal. All Rights Reserved.

#include "WordFilter.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"


#define WORD_FILTER_PACE 1

WordFilter* WordFilter::Instance = nullptr;


WordNode::WordNode()
	: Character(TEXT(""))
	, Parent(nullptr)
	, bComplete(false)
{

}

WordNode::WordNode(WordNode* InParent, FString InCharacter)
	: Character(InCharacter)
	, Parent(InParent)
	, bComplete(false)
{

}

WordNode* WordNode::GetParent()
{
	return Parent;
}

WordNode* WordNode::FindChild(FString& NextCharacter)
{
	map<FString, WordNode>::iterator TreeMapIt = WorldNodeMap.find(NextCharacter);
	if (TreeMapIt == WorldNodeMap.end())
	{
		return nullptr;
	}
	else
	{
		return &TreeMapIt->second;
	}
}

WordNode* WordNode::InsertChild(FString& NextCharacter)
{
	if (!FindChild(NextCharacter))
	{
		WorldNodeMap.insert(pair<FString, WordNode>(NextCharacter, WordNode(this, NextCharacter)));
		return &(WorldNodeMap.find(NextCharacter)->second);
	}
	return nullptr;
}


WordNode* WordTree::Insert(const FString& Keyword)
{
	return Insert(&EmptyRoot, Keyword);
}

WordNode* WordTree::Insert(const TCHAR* Keyword)
{
	return Insert(FString(Keyword));
}

WordNode* WordTree::Find(FString& Keyword)
{
	return Find(&EmptyRoot, Keyword);
}

void WordTree::Reset()
{
	Count = 0;
}

WordNode* WordTree::Insert(WordNode* Parent, const FString& Keyword)
{
	if (Keyword.Len() == 0)
	{
		if (Parent && Parent->GetParent())
		{
			Parent->bComplete = true;
		}

		return nullptr;
	}

	FString FirstChar = Keyword.Left(WORD_FILTER_PACE);
	
	WordNode* FirstNode = Parent->FindChild(FirstChar);
	if (FirstNode == NULL)
	{
		return InsertBranch(Parent, Keyword);
	}

	FString RestChar = Keyword.Mid(WORD_FILTER_PACE, Keyword.Len());
	return Insert(FirstNode, RestChar);
}

WordNode* WordTree::InsertBranch(WordNode* Parent, const FString& Keyword)
{
	FString FirstChar = Keyword.Left(WORD_FILTER_PACE);

	WordNode* FirstNode = Parent->InsertChild(FirstChar);
	if (FirstNode != nullptr)
	{
		FString RestChar = Keyword.Mid(WORD_FILTER_PACE, Keyword.Len());
		if (!RestChar.IsEmpty())
		{
			return InsertBranch(FirstNode, RestChar);
		}
		else
		{
			FirstNode->bComplete = true;
		}
	}
	return nullptr;
}

WordNode* WordTree::Find(WordNode* Parent, const FString& Keyword)
{
	FString FirstChar = Keyword.Left(WORD_FILTER_PACE);

	WordNode* FirstNode = Parent->FindChild(FirstChar);
	if (FirstNode == nullptr)
	{
		if (Parent && Parent->bComplete)
		{
			return Parent;
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		++Count;	
	}

	FString RestChar = Keyword.Mid(WORD_FILTER_PACE, Keyword.Len());
	if (FirstNode->WorldNodeMap.empty())
	{
		return FirstNode;
	}

	if (Keyword.Len() == WORD_FILTER_PACE)
	{
		if (FirstNode->bComplete)
		{
			return FirstNode;
		}
		else
		{
			return nullptr;
		}	
	}

	return Find(FirstNode, RestChar);
}


WordFilter& WordFilter::Get()
{
	if (!Instance)
	{
		Instance = new WordFilter;
	}
	
	return *Instance;
}

void WordFilter::Release()
{
	if (Instance)
	{
		delete Instance;
	}
	Instance = nullptr;
}

void WordFilter::LoadConfig(const TCHAR* Directory)
{
	TArray<FString> FileNames;

	IFileManager::Get().FindFiles(FileNames, Directory, TEXT("filter"));

	for (int32 Index = 0; Index < FileNames.Num(); Index++)
	{
		const FString& Filename = FPaths::Combine(Directory, *FileNames[Index]);
		
		TArray<FString> Result;
		FFileHelper::LoadFileToStringArray(Result, *Filename);

		for (const FString& Raw : Result)
		{			
			Tree.Insert(Raw);
		}
	}
}

void ReplaceString(FString& Source, int32 Start, int32 Num, const TCHAR Char)
{
	int32 Length = Source.Len();
	for (int32 Index = Start; Index < Start + Num; Index++)
	{
		if (Index >= Length)
		{
			continue;
		}

		Source[Index] = Char;
	}
}

void WordFilter::Censor(FString& Source)
{
	int32 Length = Source.Len();
	for (int32 Index = 0; Index < Length; Index++)
	{
		Tree.Reset();

		FString SubString = Source.Mid(Index, Length - Index);
		if (Tree.Find(SubString) != nullptr)
		{
			ReplaceString(Source, Index, Tree.Count, '*');
		}
	}
}

bool WordFilter::Check(const FString& Source)
{
	int32 Length = Source.Len();
	for (int32 Index = 0; Index < Length; Index++)
	{
		Tree.Reset();

		FString SubString = Source.Mid(Index, Length - Index);
		if (Tree.Find(SubString) != nullptr)
		{
			return false;
		}
	}

	return true;
}