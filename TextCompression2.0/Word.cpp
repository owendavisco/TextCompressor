#include <string>
#include <iostream>
using namespace std;

#include "Word.h"

//Full constructor that will instanciate all values of a Word
Word::Word(string s, int c, bool t, bool u)
{
	word = s;
	wordCount = c;
	isCaps = t;
	isUpper = u;
}

//Constructor that will allow one string as the basis for the Word
Word::Word(string s)
{
	word = s;
	wordCount = 1;
	isCaps = false;
	isUpper = false;
}

//Default constructor
Word::Word()
{
	word = "";
	wordCount = 1;
	isCaps = false;
	isUpper = false;
}

//Operator will take this Word's string and compare it to the otherWord's string
//and return whether they are the same Word
bool Word::operator==(Word otherWord)
{
	return (toUpper(this->getString()).compare(toUpper(otherWord.getString())) == 0);
}

//Operator will take the inverse of == and return its truth value
bool Word::operator!=(Word otherWord)
{
	return !(*this == otherWord);
}

bool Word::operator<(Word otherWord)
{
	return(this->getWordCount() < otherWord.getWordCount());
}

bool Word::operator>(Word otherWord)
{
	return(this->getWordCount() > otherWord.getWordCount());
}

bool Word::operator<=(Word otherWord)
{
	return(*this < otherWord || *this == otherWord);
}

bool Word::operator>=(Word otherWord)
{
	return(*this > otherWord || *this == otherWord);
}

//Operator will take another Word's string values and append it this's string value
//and will return this
Word Word::operator+(Word append)
{
	this->setString(this->getString() + append.getString());
	return *this;
}

//Operator will take a string and append it onto this Word's string value
Word Word::operator+(string append)
{
	this->setString(this->getString() + append);
	return *this;
}

//Operator will take a char and append it onto this Word's string value
Word Word::operator+(char append)
{
	this->setString(this->getString() + append);
	return *this;
}

//Used to convert a string to upperCase so that it can be compared ignoring case
string Word::toUpper(string upperWord)
{
	for (int n = 0; upperWord[n] != '\0'; n++)
	{
		upperWord[n] = toupper(upperWord[n]);
	}
	return upperWord;
}

//Clear the values and set them to the default state
void Word::clear()
{
	word = "";
	wordCount = 1;
	isCaps = false;
	isUpper = false;
}

//Getters and setters...
string Word::getString()
{
	return word;
}

void Word::setString(string newString)
{
	word = newString;
}

int Word::getWordCount()
{
	return wordCount;
}

void Word::setWordCount(int newCount)
{
	wordCount = newCount;
}

void Word::incrementWordCount(int countChange)
{
	wordCount = wordCount + countChange;
}

bool Word::isCapital()
{
	return isCaps;
}
void Word::setCapital(bool cap)
{
	isCaps = cap;
}

bool Word::isUpperCase()
{
	return isUpper;
}
void Word::setUpperCase(bool newCase)
{
	isUpper = newCase;
}
