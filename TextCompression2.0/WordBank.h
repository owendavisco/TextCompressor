#ifndef WORDBANK_H
#define WORDBANK_H

#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#include <fstream>
#include <queue>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
using namespace std;
#include "Word.h"

class WordBank
{

private:
	unordered_map<string, Word> wordMap;
	int size;

public:
	WordBank();
	WordBank(int);

	int getWordCount(string);
	void setWordCount(string s, int count);

	unordered_map<string, Word>::iterator find(string s);
	bool exists(string s);

	unordered_map<string, Word> getMap();
	vector<Word> getVectorList();
	vector<Word> getSortedVector();

	void insert(string s);
	void insert(string s, Word);
	void remove(string s);
};

#endif