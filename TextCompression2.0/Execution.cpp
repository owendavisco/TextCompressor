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

unsigned t0 = clock();

#include "WordBank.h"
#include "Word.h"
#include "Compressor.h"
string toUpper(string upperstring);
bool checkRedundancy(WordBank & bank, string s);
void readFile(string);
void outFile(vector<Word>, string);
vector<Word> heapSort(vector<Word>);

//unordered_map<string, int, bool, int> heapSort(unordered_map<string, int, bool, int>);

int main(int argc, const char* argv[])
{
	//If the user has not inputted enough information, tell them the correct usage
	if (argc <= 1)
	{
		cout << "USAGE: \"filename\".txt or \"filename\".cmp is required\n";
		return 0;
	}
	string fileName = argv[1];

	//And if the user did not input the correct file extension tell them the correct usage
	if (toUpper(fileName).find(".TXT") == -1 && toUpper(fileName).find(".CMP") == -1)
	{
		cout << "USAGE: \"filename\".txt or \"filename\".cmp are the only proper formats\n";
		return 0;
	}

	if (toUpper(fileName).find(".TXT") == -1)
	{
		readFile(fileName);
		return 0;
	}

	Compressor compressor = *new Compressor(argv[1]);

	compressor.compressFile();

	/*
	    //Create a stream for the file to read given from the first argument
	    fstream file;
	    file.open(argv[1]);

	    //Create the unordered_map bank of strings for which we will be reading 
	    WordBank stringBank;

	    queue<string> fileOrder;

	    char currentChar = currentChar = file.get();
	    float timeCheck = 0;
	    Word tempWord;
	    string temp;
	    while (currentChar != EOF)
	    {
		    //currentChar = file.get();
		    while (toupper(currentChar) >= 'A' && toupper(currentChar) <= 'Z')
		    {
			    tempWord = tempWord + toupper(currentChar);
			    currentChar = file.get();
		    }

		    if (!stringBank.exists(tempWord.getString()))
		    {
			    stringBank.insert(tempWord.getString(), tempWord);
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

		    unsigned t1 = clock();
		    fileOrder.push(tempWord.getString());
		    fileOrder.push(temp);
		    timeCheck = timeCheck + ((float)clock() - t1) / CLOCKS_PER_SEC;

		    temp.clear();
		    tempWord.clear();

		    
		    if (toupper(currentChar) >= 'A' && toupper(currentChar) <= 'Z')
		    {
			    //Keep reading in letters until the entire string has been constructed
			    while (toupper(currentChar) >= 'A' && toupper(currentChar) <= 'Z')
			    {
				    temp = temp + currentChar;
				    currentChar = file.get();
			    }
			    if (!stringBank.exists(toUpper(temp)))
			    {
				    stringBank.insert(toUpper(temp), *new Word(temp));
			    }
			    else
			    {
				    stringBank.setWordCount(toUpper(temp), stringBank.getWordCount(toUpper(temp)) + 1);
			    }
			
			    temp.clear();
			    tempWord.clear();
		    }
            
		

	    }
	    file.close();

	    outFile(heapSort(stringBank.getVectorList()), fileName);
    */
	cout << "Execution Time - " << ((float)clock() - t0) / CLOCKS_PER_SEC << endl;
	//cout << "execution Time From if - " << timeCheck << endl;
	return 0;
}

void readFile(string nameOfFile)
{
	ifstream inFile;
	inFile.open(nameOfFile);
	string temp;

	while (getline(inFile, temp))
	{
		cout << temp << "\n";
	}
}

void outFile(vector<Word> vectorList, string nameOfFile)
{
	nameOfFile = nameOfFile.substr(0, nameOfFile.length() - 3) + "cmp";
	cout << "\n OutFile Name - " << nameOfFile << "\n";
	ofstream outFile;
	outFile.open(nameOfFile);

	outFile << "string/Character       - string count" << "\n\n";
	outFile << setiosflags(ios::left);

	//Trabankerse the stringBank and output the string information to the user
	for (unsigned int i = vectorList.size()-1; i > 0; i--)
	{
		outFile << setw(20) << vectorList.at(i).getString() << " - " << (vectorList.at(i).getWordCount());
		outFile << endl;
	}

	outFile.close();
}

vector<Word> heapSort(vector<Word> v)
{
	make_heap(v.begin(), v.end());
	sort_heap(v.begin(), v.end());

	return v;
}

//Basic upperCase conversion to be used in comparisons
string toUpper(string upperstring)
{
	for (int n = 0; upperstring[n] != '\0'; n++)
	{
		upperstring[n] = toupper(upperstring[n]);
	}
	return upperstring;
}
