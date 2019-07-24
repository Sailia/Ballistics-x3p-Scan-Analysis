// openBin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h" //why is this here
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <sstream>
//#include <direct.h>
#include "stdafx.h"
#include "zipper.h"
#include "unzipper.h"
#include "elzip.hpp"
#include <experimental/filesystem>
#include <map>
#include <math.h>

using namespace ziputils;
namespace fs = std::experimental::filesystem;

/* Parameters:
	string fileName:
		path of file to be read
   
   Result:
	returns string formatted content of fileName
*/
std::string getFile(std::string fileName) {
	std::string buffer;
	char c;

	std::ifstream in(fileName);   
	if (!in) { 
		std::cout << fileName << " not found";   exit(1); 
	}
	while (in.get(c)) buffer += c;
	in.close();

	return buffer;
}

/* Parameters:
	const string &text:
		pointer to string containing xml content
	string tag:
		tag name
   
   Result:
	returns vector of values, from every instance of tag, in string format
*/
std::vector<std::string> getData(const std::string &text, std::string tag)
{
	std::vector<std::string> collection;
	unsigned int pos = 0, start;

	while (true)
	{
		start = text.find("<" + tag, pos);   if (start == std::string::npos) return collection;
		start = text.find(">", start);
		start++;

		pos = text.find("</" + tag, start);   if (pos == std::string::npos) return collection;
		collection.push_back(text.substr(start, pos - start));
	}
}

/* Parameters:
	string &text:
		pointer to text for parsing

   Result:
	removes tag brackets from text
*/
void stripTags(std::string &text)
{
	unsigned int start = 0, pos;

	while (start < text.size())
	{
		start = text.find("<", start);    if (start == std::string::npos) break;
		pos = text.find(">", start);    if (pos == std::string::npos) break;
		text.erase(start, pos - start + 1);
	}
}

/* Parameters:
	string unzipFile:
		path to file to be decompressed
	string outputPath:
		path to output folder/directory

   Result: 
	decompresses unzipFile to outputPath
*/
void unzip(std::string unzipFile, std::string outputPath)
{
	unzipper zipFile;
	zipFile.open(unzipFile.c_str());
	auto filenames = zipFile.getFilenames();

	for (auto it = filenames.begin(); it != filenames.end(); it++)
	{
		std::string filePath = outputPath +"/"+ *it;
		std::ofstream placeInFolder{ filePath,std::ios::out | std::ios::binary };
		zipFile.openEntry((*it).c_str());
		zipFile >> placeInFolder;

		placeInFolder.close();
	}
}

/* Parameters:
	string folderPath:
		path to folder

   Result:
	returns vector of names of files within folderPath
*/
std::vector<std::string> fileNames(std::string folderPath) {
	std::vector<std::string> nameList;
	for (const auto & entry : fs::directory_iterator(folderPath)) {
		nameList.push_back(entry.path().string());
		std::cout << "File found: " + entry.path().string() << std::endl;
	}
	return nameList;
}

/* Parameters:
	string xmlFile:
		path to xml file to search through
	string dimTag:
		name of tag, with an integer value, to search for (i.e. "Size X")
		assumed only one instance of tag

   Result:
	returns integer value stored in xmlFile for dimTag
*/
int getDim(std::string xmlFile, std::string dimTag) {
	int dim = 0;;

	std::string text = getFile(xmlFile);
	std::vector<std::string> allXTag = getData(text, dimTag);
	for (std::string &s : allXTag)
	{
		stripTags(s);
		std::stringstream toInt(s);
		toInt >> dim;
	}

	return dim;
}

/* Parameters:
	vector<double> x:
		double vector containing x values
	vector<double> y:
		double vector containing corresponding y values

   Result:
	returns linear regression slope for given x and y values as a double
*/
double regressionSlope(std::vector<double> x, std::vector<double> y) {
	if (x.size() != y.size()) {
		throw "inequal vector sizes";
	}
	double n = x.size();

	double avgX = (n*(n + 1)) / 2;
	double avgY = 0;

	for (double entry : y) {
		if (!(entry != entry)) {
			avgY += entry;
		}
	}

	avgY /= n;

	double numerator = 0.0;
	double denominator = 0.0;

	for (int i = 0; i < n; ++i) {
		numerator += (x[i] - avgX) * (y[i] - avgY);
		denominator += (x[i] - avgX) * (x[i] - avgX);
	}
	return numerator / denominator;
}

/* Parameters:
	string tempStorageFilePath:
		path to decompressed contents of x3p file

   Result:
	temporarily stores data stored within x3p's data.bin within vector<vector<double>> dataMatrix
	outputs percent of missing values from nine blocks of dataMatrix to console
	outputs 6 linear regression slopes, each consisting of 3 consecutive columns of data, to console
*/
void processDataMatrix(std::string tempStorageFilePath) { //takes in string of filepath and output of filepath
	std::vector<std::vector<double>> dataMatrix;
	int xDim = getDim(tempStorageFilePath + "/main.xml", "SizeX"), yDim = getDim(tempStorageFilePath + "/main.xml", "SizeY"); //set x and y dimension to dimensions found in XML file
	std::vector<float> colNanPercent(xDim, 0), rowNanPercent(yDim, 0); // var of percent of missing vals

	double data;

	std::ifstream input;
	int xPartition = xDim / 3, yPartition = yDim / 3, xCount = 0, yCount = 0, totalMissing = 0;
	double nanSum[3][3] = { {0,0,0},{0,0,0},{0,0,0} };
	

	input.open(tempStorageFilePath + "/bindata/data.bin", std::ios::binary); // reads binary file and converts binary (ios::binary, overloads)

	//include bool checks on file in separate method
	//iterators
	for (int i = 0; i < yDim; ++i) { //while we are not at the end of the file
		if (i >= yPartition && i < (yPartition * 2)) {
			yCount = 1;
		}
		else if (i >= yPartition * 2) {
			yCount = 2;
		}
		xCount = 0;
		for (int j = 0; j < xDim; ++j) {
			if (j >= xPartition && j < (xPartition * 2)) {
				xCount = 1;
			}
			else if (j >= xPartition * 2) {
				xCount = 2;
			}
			if (i == 0) {
				std::vector<double> dataCol;
				dataMatrix.push_back(dataCol);
			}
			input.read((char*)&data, sizeof(data)); // read next line into "data"
			dataMatrix[j].push_back(data);

			if(dataMatrix[j][i] != dataMatrix[j][i]) {
				nanSum[xCount][yCount]++;
				totalMissing++;
			}
			if (i + 1 == yDim) {
				colNanPercent[j] /= yDim;
			}
			
		}
		rowNanPercent[i] /= xDim;
	}
	input.close(); //close input file

	std::cout << "Block % Missing: " << std::endl;

	std::cout << "|" << std::setw(10) << roundf(((nanSum[0][0] / xPartition) / yPartition)* 100 ) / 100;
	std::cout << "|" << std::setw(10) << roundf(((nanSum[1][0] / xPartition) / yPartition)* 100 ) / 100;
	std::cout << "|" << std::setw(10) << roundf(((nanSum[2][0] / (xDim - 2 * xPartition)) / (yDim - 2 * yPartition)) * 100) / 100 << "|" << std::endl;

	std::cout << "|" << std::setw(10) << roundf(((nanSum[0][1] / xPartition) / yPartition) * 100) / 100;
	std::cout << "|" << std::setw(10) << roundf(((nanSum[1][1] / xPartition) / yPartition) * 100) / 100;
	std::cout << "|" << std::setw(10) << roundf(((nanSum[2][1] / (xDim - 2 * xPartition)) / (yDim - 2 * yPartition)) * 100) / 100 << "|" << std::endl;

	std::cout << "|" << std::setw(10) << roundf(((nanSum[0][2] / xPartition) / yPartition) * 100) / 100;
	std::cout << "|" << std::setw(10) << roundf(((nanSum[1][2] / xPartition) / yPartition) * 100) / 100;
	std::cout << "|" << std::setw(10) << roundf(((nanSum[2][2] / (xDim - 2 * xPartition)) / (yDim - 2 * yPartition)) * 100) / 100 << "|" << std::endl;

	std::vector<double> x,leftPartition, rightPartition;
	for (int j = 0; j < 9; ++j) {
		for (int i = 0; i < yDim; ++i) {
			x.push_back((double)i);
			leftPartition.push_back(dataMatrix[xPartition - 4 + j][i]);
			rightPartition.push_back(dataMatrix[2*xPartition - 4 + j][i]);
		}
	}

	std::cout << "Left Slope: " << std::setw(10) << regressionSlope(x, leftPartition) << std::endl;
	std::cout << "Right Slope: " << std::setw(10) << regressionSlope(x, rightPartition) << std::endl;
}

bool displayDebug = false; //whether to output debug info to console
std::string filePath, tempPath = "./tempdir"; //dataPath = path to data directory, tempPath = path to temporary decompressed data storage directory
std::vector<std::string> x3pNames;

int main(int argc, char *argv[])
{
	bool runExtract = true;
	if (argc <= 1 || argc > 4) {
		std::cout << "ERROR: Incorrect arguments. Please use -help command to display commands" << std::endl;
		runExtract = false;
	}
	else {
		if (argc == 2) {
			std::string isHelp = argv[1];
			if (isHelp == "-help") {
				std::cout << "-debug : display debug information" << std::endl;
				std::cout << "-extractdir=\"<directory path>\" : specify directory for temporarily storing decompressed data, exclude '<>'. omit to default to \"./tempdata\"" << std::endl;
				std::cout << "\"<file path>\" : specify path to x3p file to be processed, exclude '<>'" << std::endl;
			}
			else {
				std::cout << "ERROR: Incorrect arguments. Please use -help command to display commands" << std::endl;
			}
			runExtract = false;
		}
		else {
			for (int i = 1; i < argc; ++i) {
				std::string tempToString = argv[i];
				if (tempToString == "-debug") {
					displayDebug = true;
				}
				else if (tempToString.find("-extractdir") != std::string::npos) {
					tempPath = tempToString.substr(tempToString.find("=") + 1);
				}
				else {
					if (tempToString.substr(tempToString.length() - 4) != ".x3p") {
						std::cout << "ERROR: Did not pass a x3p file" << std::endl;
						runExtract = false;
					}
					else {
						filePath = tempToString;
					}
				}
			}
		}
	}
	if (runExtract) {
		std::cout << filePath << std::endl;
		bool isOpen = elz::extractZip(displayDebug, filePath, tempPath);
		if (isOpen) {
			processDataMatrix(tempPath);
			for (const auto & entry : fs::directory_iterator(tempPath)) {
				fs::remove_all(fs::path(entry));
			}
		}
		else {
			std::cout << "ERROR: invalid x3p file" << std::endl;
		}
	}
	//int x3pIndex = 0;
	//if (argc == 2 && argv[1] == "-help") {
	//	std::cout << "openBin <\"-debug\" to show debug information, omit otherwise> <path to temporary storage directory> <path to file to decompress>" << std::endl;
	//	std::cout << "i.e. \"openBin -debug C:/Documents/tempFolder C:/Documents/dataFolder/bulletscan.x3p\"" << std::endl;
	//}
	//else if (argc >= 2 && argc <= 4) {
	//	bool runExtract = true;
	//	if (argc == 2) {
	//		x3pIndex = 1;
	//		/*std::string dataPath = argv[1];
	//		if (dataPath.substr(dataPath.length() - 4) != ".x3p") {
	//			std::cout << "ERROR: Did not pass a x3p file" << std::endl;
	//			runExtract = false;
	//		}*/
	//	}
	//	else if (argc == 3) {
	//		x3pIndex = 2;
	//		if (argv[1] = "-debug") {
	//			displayDebug = true;
	//		}
	//		else {
	//			std::string dataPath = argv[2];
	//			if (dataPath.substr(dataPath.length() - 4) != ".x3p") {
	//				std::cout << "ERROR: Did not pass a x3p file" << std::endl;
	//				runExtract = false;
	//			}
	//		}
	//	}
	//		elz::extractZip(displayDebug, dataPath + "/" + file + ".x3p", tempPath);
	//		processDataMatrix(tempPath);
	//		for (const auto & entry : fs::directory_iterator(tempPath)) {
	//			fs::remove_all(fs::path(entry));
	//		}

	//x3pNames = fileNames(dataPath); //get list of paths to each file

	//for (std::string path : x3pNames) {
	//	elz::extractZip(displayDebug, path, tempPath);
	//	processDataMatrix(tempPath);
	//	for (const auto & entry : fs::directory_iterator(tempPath)) {
	//		fs::remove_all(fs::path(entry));
	//	}
	//}

	//processDataMatrix("C:/Users/tji/source/repos/openBin/openBin/testzipoutput", "C:/Users/tji/source/repos/openBin/openBin/testfolder/testresults.txt");

	//elz::extractZip("C:\\Users\\tji\\source\\repos\\openBin\\openBin\\testfolder\\HS44-Barrel_6-Bullet_1-Land_3.x3p", "C:\\Users\\tji\\source\\repos\\openBin\\openBin\\testzipoutput");

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	//_mkdir("C:/Users/tji/source/repos/openBin/openBin/testfolder");

	//Push Data into matrix

	//int xDim = 3671, yDim = 1023;
	//std::string inputTxt = "C:/Users/tji/source/repos/openBin/openBin/data.bin";
	////std::string outputTxt = "C:/Users/tji/source/repos/openBin/openBin/numbers.txt";

	//std::vector<std::vector<double> > dataMatrix;
	//double data;
	//std::ifstream input;
	////std::ofstream output;

	//input.open(inputTxt, std::ios::binary); // reads binary file and converts binary (ios::binary, overloads)
	////output.open(outputTxt); // opening what we're writing to

	////include bool checks on file in separate method
	////iterators
	//for(int i = 0; i < yDim; ++i) { //while we are not at the end of the file
	//	std::vector<double> dataRow;
	//	dataMatrix.push_back(dataRow);
	//	for (int j = 0; j < xDim; ++j) {
	//		input.read((char*)&data, sizeof(data)); // read next line into "data"
	//		dataMatrix[i].push_back(data);
	//		//std::cout << std::setw(10) << dataMatrix[i][j];
	//		/*if (dataMatrix[i][j] != dataMatrix[i][j]) {
	//			output << std::setw(10) << 0;
	//		}*/
	//		/*else {
	//			output << std::setw(10) << dataMatrix[i][j];
	//		}*/
	//	}
	//	//std::cout << std::endl;
	//	//output << std::endl;
	//}
	//input.close(); //close input file
	//output.close();// close output file 

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	//Read xml

	/*std::string inputTxt = "C:/Users/tji/source/repos/openBin/openBin/main.xml";
	std::string tag = "SizeX";
	int yDim = 0;

	std::string text = getFile(inputTxt);
	std::vector<std::string> all = getData(text, tag);
	for (std::string &s : all)
	{
		stripTags(s);
		std::stringstream toInt(s);
		toInt >> yDim;
	}

	std::cout << yDim << std::endl;*/

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	//Make and read bin file

	//std::vector<float> data; // vector stores floats in "data"
	//std::ifstream f; // declare variable for ifstream to read from "testnum.txt" file
	//f.open("D:/SPIRE-EIT/Ballistics/openBin/openBin/testnum.txt"); // opening "testnum" and reading from it 
	//while (!f.eof()) // To get you all the lines
	//{
	//	std::string line; //wrapping all the lines into floats
	//	std::string::size_type line_sz;     // alias of size_t
	//	std::getline(f, line);
	//	//temp.push_back(line);
	//	float data_val = std::stof(line, &line_sz); //convert line and string size type and storing in "data_val" as float
	//	data.push_back(float(data_val)); //storing all the floats in the data vector
	//}
	//f.close();

	//std::ofstream fout; //writer stream

	//fout.open("D:/SPIRE-EIT/Ballistics/openBin/openBin/test1.bin", std::ios::binary || std::ios::out); // open write mode , text file

	//for (unsigned int i = 0; i < data.size(); i++)      //array of floats
	//{
	//	//fout << data[i];
	//	fout.write((char*)&data[i], sizeof(float));  // converting into binary
	//}
	//fout.close();//close file

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
