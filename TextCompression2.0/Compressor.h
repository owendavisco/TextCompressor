// Owen Davis
// Compressor.h

#ifndef COMPRESSOR_H
#define COMPRESSOR_H

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
#include "WordBank.h"
#include "Word.h"

class Compressor
{
private:
	//Strings to keep track of specific file names
	string fileName, fileKey, fileCMP;

	//fileInput will be used to preserve file order of words
	queue<string> fileInput;

	//InsertionMap will be used to insert data based on that stringBank
	unordered_map<string, unsigned char> insertionMap;

	//The lookupTable will used to decode a .cmp file and convert it into normal words
	unordered_map<unsigned char, string> lookupTable;

	//Some utility functions used to create the lookupTable and to upper checks
	void generateLookupTable(string keyName);
	string toUpper(string s);
public:
	//Constructors based on what the user is going to do
	Compressor(string nameOfFile);
	Compressor(string nameOfFile, string keyName);
	Compressor();

	//Function that will traverse a file and find the top 255 words and return that
	WordBank generateTopWords();
	WordBank generateTopWords(string nameOfFile);

	//Functions to generate the .key file
	string generateKey();
	string generateKey(string nameOfFile);

	//Functions to compress a file
	string compressFile();
	string compressFile(string nameOfFile);

	//Functions to decompress a file
	string decompressFile();
	string decompressFile(string nameOfFile, string keyName);

	/*File name, key and cmp getters and setters*/
	string getFileName();
	void setFileName(string nameOfFile);

	string getKeyName();
	void setKeyName(string keyName);
	
	string getCMPName();
	void setCMPName(string cmpName);
	/*end getters and setters*/
};

#endif
