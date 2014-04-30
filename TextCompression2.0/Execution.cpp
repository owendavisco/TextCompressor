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

//Enum used for differentiating seperate compression types
//     INDIV - compress all the files individually
//     ONECMP - compress all the files in to one single cmp file
//     DECOMP - decompress files
//     NULLTYPE - null type (something may have gone wrong, or unspecified)
enum CompressionType{ INDIV, ONECMP, DECOMP, NULLTYPE };

int main(int argc, const char* argv[])
{
	//If the user has not inputted enough information, tell them the correct usage
	if (argc <= 1)
	{
		cout << "USAGE: \"filename\".txt or \"filename\".cmp is required\n";
	}
	else if (argc > 10)
	{
		cout << "Too Many Files Have Been Provided - Max 10 Files\n";
	}
	else
	{
		//Create a varaible for the compression type
		CompressionType type = NULLTYPE;

		//Create a way to check for user errors with the variable problem
		bool problem = false;

		//Create vectors to hold the input file names and file sizes
		vector<string> inputFiles(argc - 1);
		vector<int> fileSizes(argc - 1);

		//Traverse the incoming arguments and add them to the vector of input files
		//     then check add their size to the vector of sizes.
		for (int i = 1; i <= argc && !problem; i++)
		{
			inputFiles.at(i) = argv[i];
			fstream fileTemp(argv[i], ios::binary | ios::ate);

			//Check if the user entered a bad or empty file, and if so exit
			if ((fileTemp.bad() || fileTemp.tellg() <= 0) && type == INDIV)
			{
				cout << "The file " << argv[i] << " doesn't exist or is empty\n";
				problem = true;
			}

			//Extensive error checking for extensions, and identification of what we will be doing
			else if (type == NULLTYPE)
			{
				//If the first argument read is a cmp, define type to decompress
				if (inputFiles.at(i).find(".cmp") > 0)
				{
					type = DECOMP;
				}

				//If the first argument read is a text file, assume individual compression for now
				if (inputFiles.at(i).find(".txt") > 0)
				{
					type = INDIV;
				}
			}

			//If the current type is decompression, then every other argument will have a .cmp or exit
			else if (type == DECOMP && inputFiles.at(i).find(".cmp") <= 0)
			{
				cout << "The arguments did not follow standards for decompression\nUSAGE: file.cmp file2.cmp file3.cmp ...\n";
				problem = true;
			}

			//if the current type is individual compression and does not contain a .txt extension
			else if (type == INDIV && inputFiles.at(i).find(".txt") <= 0)
			{
				//Check if this is not the last argument, and if it is not then tell the user how to use the program
				if (argc != i)
				{
					cout << "The arguments did not follow standards for compression\nUSAGE: file.txt file2.txt file3.txt ...\n";
					problem = true;
				}

				//Otherwise if this is the last argument, and it has a .cmp,
				//    then the compression type will become compress all into one cmp
				else if (argc == i && inputFiles.at(i).find(".cmp") > 0)
				{
					type = ONECMP;
				}
			}

			//Calculate the file size and store it in fileSizes
			fileSizes.at(i) = fileTemp.tellg();
		}

		//If the user did not input correct file extensions, tell them proper usage
		if (type == NULLTYPE)
		{
			cout << "USAGE: file.txt file2.txt file3.txt ...\nor file.cmp file2.cmp file3.cmp ...\nor file.txt file2.txt file3.cmp\n";
		}

		//Otherwise if there was not a problem we can begin compression
		else if (!problem)
		{
			//Create the compressor since we might be doing some compressing
			Compressor compressor;

			if (type == INDIV)
			{
				for each (string fileName in inputFiles)
				{
					compressor.compressFile(fileName);
				}
			}
			else if (type == ONECMP)
			{
				string outFileName = inputFiles.back();
				inputFiles.pop_back();
				for each (string fileName in inputFiles)
				{
					compressor.compressFile(outFileName, (ios::app | ios::binary));
				}
			}
			else
			{
				for each (string fileName in inputFiles)
				{
					compressor.decompressFile(fileName);
				}
			}
		}
	}


	/*
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
	*/
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
