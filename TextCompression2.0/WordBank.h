// Owen Davis
// WordBank.h

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
	//Variables to store the map of words, and the size of that map
	unordered_map<string, Word> wordMap;
	int size;

public:
	//Constructors
	WordBank();
	WordBank(int sizeOfBank);

	//Getter and setter for the number of word occurences
	int getWordCount(string);
	void setWordCount(string s, int count);

	//Function to find a string inside of the map, and return its location
	unordered_map<string, Word>::iterator find(string s);

	//Check if a string is inside the map
	bool exists(string s);

	//Get map, and get the map as a vector
	unordered_map<string, Word> getMap();
	vector<Word> getVectorList();

	//Get a sorted version of the map (big to small)
	vector<Word> getSortedVector();

	//Inserting strings and removing
	void insert(string s);
	void insert(string s, Word);
	void remove(string s);
};

#endif