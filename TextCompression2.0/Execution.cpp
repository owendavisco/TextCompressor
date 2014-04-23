// Owen Davis
// 4-22-2014
// CS 3060
//
// This program will take a file (either .txt or .cmp) and
// either compress or decompress that file. This program will
// make us of the Compressor class in order to compress/decompress

#include <time.h> 
using namespace std;

unsigned t0 = clock();

#include "WordBank.h"
#include "Word.h"
#include "Compressor.h"

string toUpper(string upperstring);

int main(int argc, const char* argv[])
{
	//If the user has not inputted enough information, tell them the correct usage
	if (argc <= 1)
	{
		cout << "USAGE: \"filename\".txt or \"filename\".cmp is required\n";
		return 0;
	}
	string fileName = argv[1];
	ifstream file(fileName, ios::binary | ios::ate);
	Compressor compressor;

	long fileSize = file.tellg();

	//If the file is empty
	if (fileSize <= 0)
	{
		cout << "File - " << fileName << " is empty\n";
	}

	//If the user has provided a .txt file
	else if (toUpper(fileName).find(".TXT") != -1)
	{
		//Output the uncompressed size
		cout << "Uncompressed - " << fileName << " " << fileSize << " bytes\n";

		//Compress the file named fileName, and then store that new file name in fileName
		fileName = compressor.compressFile(fileName);

		//Grab this new file size and output it to the user, and then its compression rate
		long fileSizeCo = (*new ifstream(fileName, ios::binary | ios::ate)).tellg();
		cout << "Compressed - " << fileName << " " << fileSizeCo << " bytes\n";
		cout << "File Compression Rate - " << setprecision(4) << ((1 - ((float)fileSizeCo / fileSize)) * 100) << "%\n";
	}
	//If the user has provided a .cmp file
	else if (toUpper(fileName).find(".CMP") != -1)
	{
		//Output the compressed file name and size, and then grab the .key file
		cout << "Compressed - " << fileName << " " << fileSize << " bytes\n";
		fileName = compressor.decompressFile(fileName, fileName.substr(0, fileName.find(".")) + ".key");

		//Grab the new decompressed file and output it to the user along with its compression rate
		long fileSizeDe = (*new ifstream(fileName, ios::binary | ios::ate)).tellg();
		cout << "Decompressed - " << fileName << " " << fileSizeDe << " bytes\n";

		cout << "File Compression Rate - " << setprecision(4) << ((1 - ((float)fileSize / fileSizeDe)) * 100) << "%\n";
	}
	//If the file does not use the correct format, tell the user
	else
	{
		cout << "USAGE: \"filename\".txt or \"filename\".cmp are the only proper formats\n";
	}

	//Output the execution time, primarily used for testing but can be useful
	cout << "\nExecution Time - " << ((float)clock() - t0) / CLOCKS_PER_SEC << " seconds" << endl;
	return 0;
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
