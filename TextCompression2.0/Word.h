// Owen Davis
// Word.h

#ifndef WORD_H
#define WORD_H

#include <string>
#include <iostream>
using namespace std;


class Word
{

	//Declare properties of the Word object that include its string value - word, 
	//the number of times that word shows up - wordCount, whether it is capital - isCaps
	//and whether it is all upper case - isUpper
private:
	string word;
	int wordCount;
	bool isCaps, isUpper;

	//Declare a to upper so that comparisons can be made ignoring case
	string toUpper(string);

public:
	//Declare Constructors depending on what data is available 
	Word(string, int, bool, bool);
	Word(string);
	Word();

	//Declare Overloads for equivalence operators for comparisons with other Words
	bool operator==(Word);
	bool operator!=(Word);

	bool operator<(Word);
	bool operator>(Word);
	bool operator<=(Word);
	bool operator>=(Word);

	//Declare overloads for the addition operator 
	//in order to append values onto the string value of Word
	Word operator+(Word);
	Word operator+(string);
	Word operator+(char);

	//String value getter and setter
	string getString();
	void setString(string);

	//Word count getter and setter
	int getWordCount();
	void setWordCount(int);
	void incrementWordCount(int);

	//Is capital getter and setter
	bool isCapital();
	void setCapital(bool);

	//Is upper getter and setter
	bool isUpperCase();
	void setUpperCase(bool);

	//Fuction to clear out values of the Word without creating a new Word
	void clear();
};

#endif