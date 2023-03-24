// Copyright 2022 CQUnreal. All Rights Reserved.

#pragma once

#include <map>

using namespace std;

class WordTree;
class WorldFilter;

class WordNode
{
private:
	friend class WordTree;

	FString Character;
	
	WordNode* Parent;

	map<FString, WordNode> WorldNodeMap;
	
	bool bComplete;

public:
	WordNode();
	WordNode(WordNode* InParent, FString InCharacter);

	FString GetCharacter() const{ return Character; };
	
	WordNode* GetParent();
	WordNode* FindChild(FString& NextCharacter);
	WordNode* InsertChild(FString& NextCharacter);
};

class WordTree
{
public:
	WordTree()
		: Count(0)
		, Pace(0)
	{

	}

	WordNode* Insert(const FString& Keyword);
	WordNode* Insert(const TCHAR* Keyword);
	WordNode* Find(FString& Keyword);

	void Reset();

private:
	friend class WordFilter;

	int32 Count;
	int32 Pace;
	WordNode EmptyRoot;

	WordNode* Insert(WordNode* Parent, const FString& Keyword);
	WordNode* InsertBranch(WordNode* Parent, const FString& Keyword);
	WordNode* Find(WordNode* Parent, const FString& Keyword);
};

class WordFilter
{
public:
	static WordFilter& Get();
	static void Release();

private:
	WordTree Tree;

	static WordFilter* Instance;

public:
	void LoadConfig(const TCHAR* Directory);
	void Censor(FString& Source);
	bool Check(const FString& Source);
};
