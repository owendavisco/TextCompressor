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

		//if (temp.compare(" ") != 0)
		//{
			fileInput.push(temp);
		//}

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

	string fileCMP = fileName.substr(0, fileName.length() - 3) + "cmp";
	cout << "--CMP OutFile Name - " << fileCMP << "\n";
	ofstream outFile;
	bool inserted = false;
	outFile.open(fileCMP);

	unordered_map<string, char>::iterator temp;
	string currentWord;

	while (!fileInput.empty())
	{
		temp = insertionMap.find(fileInput.front());

		while (temp == insertionMap.end() && !fileInput.empty())
		{
			currentWord = currentWord + fileInput.front();
			fileInput.pop();
			temp = insertionMap.find(fileInput.front());

			inserted = true;
		}

		if (inserted)
		{
			outFile.put(insertionMap.find("<0>")->second);
			outFile << currentWord;
			outFile.put(insertionMap.find("<0>")->second);
			inserted = false;
		}

		else
		{
			outFile.put(temp->second);
			//cout << "Character inserted -- " << (int)temp->second << endl;
		}
		//cout << "Current Word -" << currentWord << endl;
		
		currentWord.clear();
		fileInput.pop();

		/*
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
		*/
	}

	return fileCMP;
}
string Compressor::compressFile(string nameOfFile)
{
	fileName = nameOfFile;
	return (this->compressFile());
}

string Compressor::compressFile(string nameOfFile, string keyName)
{
	fileName = nameOfFile;
	fileKey = keyName;
	return (this->compressFile());
}

string Compressor::getFileName()
{

}
void Compressor::setFileName(string nameOfFile)
{

}

void Compressor::generateLookupTable(string keyName)
{
	fstream infile;
	infile.open(keyName);
	char currentChar;
	string currentWord = "";

	if (!(keyName.substr(keyName.find("."), keyName.length())).compare("key") == 0 && infile.good())
	{
		cout << "FUNCTION REQUIRES .KEY FILETYPE\n";
		return;
	}
	else if (!infile.good())
	{
		cout << "FILE DOES NOT EXIST OR DID NOT OPEN CORRECTLY\n";
		return;
	}

	currentChar = infile.get();
	while (currentChar != EOF)
	{
		while (currentChar != '-')
		{
			currentWord = currentWord + currentChar;
		}

		lookupTable.emplace(infile.get(), currentWord);

		infile.get();
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