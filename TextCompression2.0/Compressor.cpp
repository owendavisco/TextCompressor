// Owen Davis
// Compressor.cpp
#include <iostream>
using namespace std;
#include "Compressor.h"
#include "Word.h"
#include "WordBank.h"

//Constructors to handle different numbers of arguments
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

//Given a string, generateTopWords will set the file name and then call generateTopWords()
WordBank Compressor::generateTopWords(string nameOfFile)
{
	fileName = nameOfFile;
	return (this->generateTopWords());
}

//generateTopWords will take a file and find the top 255 words in that file
WordBank Compressor::generateTopWords()
{
	//Create a file stream to be read named file
	fstream file;
	file.open(fileName);

	//If the file doesn't work, tell the user
	if (file.bad())
	{
		cout << "KEY FILE GENERATION FAILED\n";
	}

	//Create a bank of words for which the input can be inserted
	WordBank stringBank;

	//Create some variables to traverse the file
	char currentChar = file.get();
	Word tempWord;
	string temp;

	//For as long as the file is not at the end
	while (currentChar != EOF)
	{
		//If the currentChar is a letter, then continue to read characters until
		//   a word has been constructed. Store that word in tempWord
		while (toupper(currentChar) >= 'A' && toupper(currentChar) <= 'Z')
		{
			tempWord = tempWord + currentChar;
			currentChar = file.get();
		}

		//If this word is not already in the stringBank, insert it
		if (!stringBank.exists(tempWord.getString()))
		{
			stringBank.insert(tempWord.getString());
		}

		//Otherwise find that Word in stringBank and increment its counter
		else
		{
			stringBank.setWordCount(tempWord.getString(), stringBank.getWordCount(tempWord.getString()) + 1);
		}

		//Until another letter is read, continue to store these characters into temp
		while (currentChar != EOF && !(toupper(currentChar) >= 'A' && toupper(currentChar) <= 'Z'))
		{
			temp = temp + currentChar;
			currentChar = file.get();
		}

		//Push the Word into fileInput
		fileInput.push(tempWord.getString());

		//If temp is not just a space, put it into the fileInput
		if (temp.compare(" ") != 0)
		{
			fileInput.push(temp);
		}

		//Clear the temp variables
		temp.clear();
		tempWord.clear();

	}
	file.close();

	return stringBank;
}

//generateKey will create the .key file
string Compressor::generateKey()
{
	//Find the top words for key construction
	WordBank stringBank = generateTopWords();

	//Create the .key file and prepare it for writing
	string keyName = fileName.substr(0, fileName.length() - 3) + "key";
	fileKey = keyName;
	ofstream outFile;
	outFile.open(keyName);

	//Grab the sorted stringBank and create some traversal variables
	vector<Word> vectorList = stringBank.getSortedVector();
	unsigned char code = 0;
	string currentWord;

	//Create the first key value of <0> which will stand for literal
	outFile << "<0>" << "-" << code << endl;
	insertionMap.emplace("<0>", code);

	code++;

	//Traverse the vectorList and outFile the word and code with "-" seperating them
	for (unsigned int i = vectorList.size() - 1; i > 0 && code < 255; i--, code++)
	{
		currentWord = vectorList.at(i).getString();

		outFile << currentWord;
		outFile.put('-');
		outFile.put(code);

		insertionMap.emplace(currentWord, code);
	}
	outFile.close();

	return keyName;
}

//Key generation given the original file name
string Compressor::generateKey(string nameOfFile)
{
	fileName = nameOfFile;
	return (this->generateKey());
}

//compressFile will take a file and produce a compressed .cmp and a related .key file
string Compressor::compressFile()
{
	generateKey();

	//Create the .cmp for insertion
	fileCMP = fileName.substr(0, fileName.length() - 3) + "cmp";
	ofstream outFile;
	bool inserted = false;
	outFile.open(fileCMP, ios::out | ios::binary);

	//Create a temp iterator to keep track of words, currentWord will
	//    be used for word constrution
	unordered_map<string, unsigned char>::iterator temp;
	string currentWord;

	//As long as the fileInput queue is not empty, check if it is in the key
	//    and insert the code, otherwise insert the literal identifier for that word
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

//Compress a file given a file name
string Compressor::compressFile(string nameOfFile)
{
	fileName = nameOfFile;
	return (this->compressFile());
}

string Compressor::decompressFile()
{
	//Create and get the .cmp file for decompression
	ifstream cmpFile;
	cmpFile.open(fileCMP, ios::in | ios::binary);
	if (cmpFile.bad())
	{
		cout << "File \"" << fileCMP << "\" is bad or does not exist\n";
		return "-1";
	}

	//Create a file for decompression with the .txt extension
	ofstream outFile;
	fileName = fileCMP.substr(0, fileCMP.length() - 3) + "txt";
	outFile.open(fileName);
	
	//Generate the lookupTable and some variables for traversing the file
	generateLookupTable(fileKey);
	unsigned char currentChar = cmpFile.get();

	//currentWord and priorWord will consist of series of bytes
	//    whether they are words or not
	string currentWord;
	string priorWord;

	//As long as the file is good
	while (cmpFile.good())
	{
		//If the literal character has been read
		if (lookupTable.find(currentChar)->second.compare("<0>") == 0)
		{
			//Continue to read charaters literally until the end literal is found
			//    and store these characters into the string currentWord
			currentChar = cmpFile.get();
			while (lookupTable.find(currentChar)->second.compare("<0>") != 0)
			{
				currentWord = currentWord + (char)currentChar;
				currentChar = cmpFile.get();
			}

			//If the priorWord is a word and the currentWord is a word, then print a space
			if ((toupper(priorWord.back()) >= 'A' && toupper(priorWord.back()) <= 'Z') && (toupper(currentWord.back()) >= 'A' && toupper(currentWord.back()) <= 'Z'))
				outFile << ' ';

			//Outfile the currentWord
			outFile << currentWord;
			
			priorWord = currentWord;
			currentChar = cmpFile.get();
			currentWord.clear();
		}

		//If the byte read is a code for a full length word, outFile the decoded word
		if (lookupTable.find(currentChar) != lookupTable.end() && lookupTable.find(currentChar)->second.compare("<0>") != 0)
		{
			if (toupper(priorWord.back()) >= 'A' && toupper(priorWord.back()) <= 'Z')
				outFile << ' ';
			outFile << lookupTable.find(currentChar)->second;
			priorWord = lookupTable.find(currentChar)->second;
			currentChar = cmpFile.get();
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

/*Getters and setters*/
string Compressor::getFileName()
{
	return fileName;
}
void Compressor::setFileName(string nameOfFile)
{
	fileName = nameOfFile;
}

string Compressor::getKeyName()
{
	return fileKey;
}
void Compressor::setKeyName(string keyName)
{
	fileKey = keyName;
}

string Compressor::getCMPName()
{
	return fileCMP;
}
void Compressor::setCMPName(string cmpName)
{
	fileCMP = cmpName;
}
/*End getters and setters*/

//Create the lookupTable which will be used for decompression
void Compressor::generateLookupTable(string keyName)
{
	//Open the key file and create variables to traverse the file
	fstream infile;
	infile.open(keyName, ios::in | ios::binary);
	unsigned char currentChar = infile.get();
	string currentWord;
	bool isNewLine = false;

	if (!infile.good())
	{
		cout << "File \"" << fileKey << "\" is bad or does not exist\n";
		return;
	}

	//As long as the file is being read
	while (infile.good())
	{
		//Continue to grab characters until '-' is reached
		while (currentChar != '-')
		{
			//Make sure not to write any new lines to the word
			if (currentChar != 13 && currentChar != 10)
			{
				currentWord = currentWord + (char)currentChar;
			}
			
			//Get the next character
			currentChar = infile.get();
		}

		//This is the first character after '-' and is the code
		currentChar = infile.get();

		//if this character is the carriage return and not a new line
		if (currentChar == 13 && !isNewLine)
		{
			//grab the next character since the carriage return and new line
			//    may be next to each other, which is not what we want
			currentChar = infile.get();
			isNewLine = true;
		}

		//add the word to the lookupTable with currentChar being its code
		lookupTable.emplace(currentChar, currentWord);

		currentChar = infile.get();
		currentWord.clear();
	}
}

//Turns a string into uppercase and returns that uppercase word
string Compressor::toUpper(string s)
{
	for (int n = 0; s[n] != '\0'; n++)
	{
		s[n] = toupper(s[n]);
	}
	return s;
}