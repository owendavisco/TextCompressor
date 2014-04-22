#include <iostream>
using namespace std;
#include "Compressor.h"
#include "Word.h"
#include "WordBank.h"

/*
string fileName, fileKey;
ifstream infile;
ofstream outfile;
queue<string> fileInput;
*/

Compressor::Compressor(string nameOfFile)
{
	fileName = nameOfFile;
}
Compressor::Compressor(string nameOfFile, string keyName)
{
	fileName = nameOfFile;
	fileKey = keyName;
}
Compressor::Compressor()
{

}

string Compressor::generateKey()
{
	fstream file;
	file.open(fileName);
	if (file.bad())
	{
		cout << "KEY FILE GENERATION FAILED\n";
	}

	//Create a bank of words for which the input can be inserted
	WordBank stringBank;

	char currentChar = file.get();
	Word tempWord;
	string temp;
	while (currentChar != EOF)
	{
		while (toupper(currentChar) >= 'A' && toupper(currentChar) <= 'Z')
		{
			tempWord = tempWord + currentChar;
			currentChar = file.get();
		}

		if (!stringBank.exists(tempWord.getString()))
		{
			stringBank.insert(tempWord.getString());
		}
		else
		{
			stringBank.setWordCount(tempWord.getString(), stringBank.getWordCount(tempWord.getString()) + 1);
		}

		while (currentChar != EOF && !(toupper(currentChar) >= 'A' && toupper(currentChar) <= 'Z'))
		{
			temp = temp + currentChar;
			currentChar = file.get();
		}

		fileInput.push(tempWord.getString());

		if (temp.compare(" ") != 0)
		{
			fileInput.push(temp);
		}

		temp.clear();
		tempWord.clear();

	}
	file.close();

	string keyName = fileName.substr(0, fileName.length() - 3) + "key";
	ofstream outFile;
	outFile.open(keyName, ios::out);

	vector<Word> vectorList = stringBank.getSortedVector();
	unsigned char code = 0;
	string currentWord;

	outFile << "<0>" << "-" << code << endl;
	insertionMap.emplace("<0>", code);

	code++;

	/*
	outFile << "<CAPS>" << "-" << code << endl;
	insertionMap.emplace("<CAPS>", code);

	code++;
	*/

	//Traverse the stringBank and output the string information to the user
	for (unsigned int i = vectorList.size() - 1; i > 0 && code < 255; i--, code++)
	{
		currentWord = vectorList.at(i).getString();

		outFile << currentWord;
		outFile.put('-');
		outFile.put(code);

		if (code == 10)
		{
			cout << "Code 10 is - " << currentWord << endl;
		}

		insertionMap.emplace(currentWord, code);
	}
	outFile.close();

	return keyName;
}

string Compressor::generateKey(string nameOfFile)
{
	fileName = nameOfFile;
	return (this->generateKey());
}

string Compressor::compressFile()
{
	cout << "Created key file - " << generateKey() << endl;

	fileCMP = fileName.substr(0, fileName.length() - 3) + "cmp";
	cout << "--CMP OutFile Name - " << fileCMP << "\n";
	ofstream outFile;
	bool inserted = false;
	outFile.open(fileCMP, ios::in | ios::binary);

	unordered_map<string, unsigned char>::iterator temp;
	string currentWord;

	while (!fileInput.empty())
	{
		temp = insertionMap.find(fileInput.front());
		
		currentWord = fileInput.front();
		temp = insertionMap.find(currentWord);
		if (temp != insertionMap.end())
		{
			outFile.put(temp->second);
		}

		else
		{
			outFile.put(insertionMap.find("<0>")->second);
			outFile << currentWord;
			outFile.put(insertionMap.find("<0>")->second);
		}

		fileInput.pop();
		currentWord.clear();
		
	}

	return fileCMP;
}
string Compressor::compressFile(string nameOfFile)
{
	fileName = nameOfFile;
	return (this->compressFile());
}

string Compressor::decompressFile()
{
	ifstream cmpFile;
	ofstream outFile;
	cmpFile.open(fileCMP, ios::in | ios::binary);
	if (cmpFile.bad())
	{
		cout << "File \"" << fileCMP << "\" is bad or does not exist\n";
		return "-1";
	}

	fileName = "UncompressedFile.txt";
	outFile.open(fileName);
	
	cout << "File with key " << fileKey << " being decompressed\n";
	generateLookupTable(fileKey);
	unsigned char currentChar = cmpFile.get();
	string currentWord;
	unordered_map<unsigned char, string>::iterator temp = lookupTable.find(currentChar);

	for (auto it = lookupTable.begin(); it != lookupTable.end(); ++it)
	{
		std::cout << "Char (int) - " << (int)it->first << ": Word - " << it->second << endl;
	}

	system("pause");
	while (cmpFile.good())
	{
		cout << "Char at the top is (int) - " << (int)currentChar << endl;
		if (lookupTable.find(currentChar)->second.compare("<0>") == 0)
		{
			currentChar = cmpFile.get();
			while (lookupTable.find(currentChar)->second.compare("<0>") != 0)
			{
				currentWord = currentWord + to_string(currentChar);
				currentChar = cmpFile.get();
			}

			currentChar = cmpFile.get();
			currentWord.clear();
		}

		if (lookupTable.find(currentChar) != lookupTable.end())
		{
			currentChar = cmpFile.get();
		}
		else
		{
			cout << "Something went wrong with char - " << (int)currentChar << endl;
		}
	}
	return fileName;
}

string Compressor::decompressFile(string nameOfCMP, string keyName)
{
	fileCMP = nameOfCMP;
	fileKey = keyName;
	return (this->decompressFile());
}

string Compressor::getFileName()
{
	return fileName;
}
void Compressor::setFileName(string nameOfFile)
{
	fileName = nameOfFile;
}

void Compressor::generateLookupTable(string keyName)
{
	fstream infile;
	infile.open(keyName, ios::in | ios::binary);
	unsigned char currentChar = infile.get();
	string currentWord;

	if (!infile.good())
	{
		cout << "FILE DOES NOT EXIST OR DID NOT OPEN CORRECTLY\n";
		return;
	}

	while (infile.good())
	{
		
		while (currentChar != '-')
		{
			currentWord = currentWord + (char)currentChar;
			currentChar = infile.get();
		}

		currentChar = infile.get();
		lookupTable.emplace(currentChar, currentWord);
		cout << "Current word is - " << currentWord << " Current Char is - " << (int)currentChar << endl;

		currentChar = infile.get();
		currentWord.clear();
	}
}

string Compressor::toUpper(string s)
{
	for (int n = 0; s[n] != '\0'; n++)
	{
		s[n] = toupper(s[n]);
	}
	return s;
}