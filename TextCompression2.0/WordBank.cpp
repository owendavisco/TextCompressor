#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;
#include "WordBank.h"
#include "Word.h"

WordBank::WordBank()
{
	wordMap = *new unordered_map<string, Word>;
}
WordBank::WordBank(int count)
{
	wordMap = *new unordered_map<string, Word>(count);
	size = count;
}

int WordBank::getWordCount(string s)
{
	unordered_map<string, Word>::iterator temp = find(s);
	if (temp != wordMap.end())
	{
		return (wordMap.find(s)->second).getWordCount();
	}
}

void WordBank::setWordCount(string s, int count)
{
	unordered_map<string, Word>::iterator temp = find(s);
	if (temp != wordMap.end())
	{
		(temp->second).setWordCount(count);
	}
}

unordered_map<string, Word>::iterator WordBank::find(string s)
{
	unordered_map<string, Word>::iterator element = wordMap.find(s);
	return element;
}

bool WordBank::exists(string s)
{
	unordered_map<string, Word>::iterator element = (wordMap.find(s));
	if (element != wordMap.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void WordBank::insert(string s)
{
	wordMap.emplace(s, *new Word(s));
}

void WordBank::insert(string s, Word wrd)
{
	wordMap.emplace(s, wrd);
}

void WordBank::remove(string s)
{
	wordMap.erase(s);
}

unordered_map<string, Word> WordBank::getMap()
{
	return wordMap;
}

vector<Word> WordBank::getVectorList()
{
	vector<Word> list;
	string temp;
	for (auto it = wordMap.begin(); it != wordMap.end(); it++)
	{
		list.push_back(it->second);
		temp.clear();
	}

	return list;
}

vector<Word> WordBank::getSortedVector()
{
	vector<Word> v =  this->getVectorList();
	make_heap(v.begin(), v.end());
	sort_heap(v.begin(), v.end());

	return v;
}