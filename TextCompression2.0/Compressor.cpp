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

	try
	{
		file.open(fileName);
		if (file.fail())
		{
			throw "FILE COULD NOT BE OPENED";
		}
	}

	catch (string s)
	{
		cout << "ERROR: " << s << "\n";
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
	outFile.open(keyName);

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

		outFile << currentWord << "-" << (char)code;

		insertionMap.emplace(currentWord, code);

        //code 10 and 13 cause a newline character so do not output an endl if they are inserted
		if (code != 10 && code != 13)
		{
			outFile << endl;
		}
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
	outFile.open(fileCMP, ios::binary);

	unordered_map<string, char>::iterator temp;
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
	ifstream cmpFile, keyFile;
	ofstream outFile;
	cmpFile.open(fileCMP);
	keyFile.open(fileKey);
	if (cmpFile.bad())
	{
		cout << "File \"" << fileCMP << "\" is bad or does not exist\n";
		return "-1";
	}
	else if (keyFile.bad())
	{
		cout << "File \"" << fileKey << "\" is bad or does not exist\n";
		return "-1";
	}

	fileName = "UncompressedFile.txt";
	outFile.open(fileName);
	
	cout << "File with key " << fileKey << " being decompressed\n";
	generateLookupTable(fileKey);
	char currentChar = cmpFile.get();
	string currentWord;
	unordered_map<char, string>::iterator temp = lookupTable.find(currentChar);
	while (cmpFile.good())
	{
		if (temp->second.compare("<0>") == 0)
		{
			currentChar = cmpFile.get();
			while (!(temp->second.compare("<0>") == 0))
			{
				currentWord = currentWord + currentChar;
				currentChar = cmpFile.get();
				temp = lookupTable.find(currentChar);
			}

			outFile << currentWord << ' ';
		}
		else
		{
			outFile << temp->second << ' ';
		}

		currentChar = cmpFile.get();
		temp = lookupTable.find(currentChar);

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
	string currentLine;

	if (!infile.good())
	{
		cout << "FILE DOES NOT EXIST OR DID NOT OPEN CORRECTLY\n";
		return;
	}

	while (infile.good())
	{
		
		infile >> currentLine;

		if (currentLine != "")
		{
			lookupTable.emplace(currentLine[currentLine.find("-") + 1], currentLine.substr(0, currentLine.find("-")));
			cout << "Current Key - " << currentLine[currentLine.find("-") + 1] << endl;
		}
		currentLine.clear();
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