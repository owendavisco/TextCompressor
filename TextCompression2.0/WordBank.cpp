// Owen Davis
// WordBank.cpp

#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;
#include "WordBank.h"
#include "Word.h"

//Constructor will create a new map, and when given a size instanciate it
WordBank::WordBank()
{
	wordMap = *new unordered_map<string, Word>;
}
WordBank::WordBank(int count)
{
	wordMap = *new unordered_map<string, Word>(count);
	size = count;
}

//getWordCount will traverse the map and then use the "Word" to find the count
int WordBank::getWordCount(string s)
{
	unordered_map<string, Word>::iterator temp = find(s);
	if (temp != wordMap.end())
	{
		return (wordMap.find(s)->second).getWordCount();
	}
	return 0;
}

//setWordCount will traverse the map and then when the "Word" is found increment the counter
void WordBank::setWordCount(string s, int count)
{
	unordered_map<string, Word>::iterator temp = find(s);
	if (temp != wordMap.end())
	{
		(temp->second).setWordCount(count);
	}
}

//find will search for a string inside the map and return its iterator
unordered_map<string, Word>::iterator WordBank::find(string s)
{
	unordered_map<string, Word>::iterator element = wordMap.find(s);
	return element;
}

//Traverse the map and check if it is found
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

/*Insert and remove values from the map*/
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
/*End insert and remove*/

//Return the map to the caller
unordered_map<string, Word> WordBank::getMap()
{
	return wordMap;
}

//Convert the map into a vector and return it
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

//Convert the map into a vector, sort that vector, and then return that vector
vector<Word> WordBank::getSortedVector()
{
	vector<Word> v =  this->getVectorList();
	make_heap(v.begin(), v.end());
	sort_heap(v.begin(), v.end());

	return v;
}