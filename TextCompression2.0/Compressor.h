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
	string fileName, fileKey;
	queue<string> fileInput;
	WordBank stringBank;
	unordered_map<string, char> insertionMap;
	unordered_map<char, string> lookupTable;

	void generateLookupTable(string keyName);
	string toUpper(string s);
public:
	Compressor(string nameOfFile);
	Compressor(string nameOfFile, string keyName);
	Compressor();

	string generateKey();
	string generateKey(string fileName);
	string generateKey(WordBank bank);

	string compressFile();
	string compressFile(string nameOfFile);
	string compressFile(string nameOfFile, string keyName);

	string decompressFile();
	string decompressFile(string nameOfFile);
	string decompressFile(string nameOfFile, string keyName);

	void outputCompressed(string nameOfFile);
	void outputKey(string nameOfFile);

	unordered_map<int, char>getKeyValues(string keyName);

	string getFileName();
	void setFileName(string nameOfFile);
};

#endif