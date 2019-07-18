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
#include <math.h>       /* round, floor, ceil, trunc */

using namespace ziputils;
namespace fs = std::experimental::filesystem;

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

void unzip(std::string unzipFilePath, std::string outputPath)
{
	unzipper zipFile;
	zipFile.open(unzipFilePath.c_str());
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

std::vector<std::string> fileNames(std::string folderPath) {
	std::vector<std::string> nameList;
	for (const auto & entry : fs::directory_iterator(folderPath)) {
		nameList.push_back(entry.path().string());
		std::cout << "File found: " + entry.path().string() << std::endl;
	}
	return nameList;
}

int getDim(std::string xmlFilePath, std::string dimTag) {
	int dim = 0;;

	std::string text = getFile(xmlFilePath);
	std::vector<std::string> allXTag = getData(text, dimTag);
	for (std::string &s : allXTag)
	{
		stripTags(s);
		std::stringstream toInt(s);
		toInt >> dim;
	}

	return dim;
}

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

	/*std::ofstream resultOutput;
	resultOutput.open(resultOutputFilePath);*/

	std::cout << "|" << std::setw(10) << roundf(((nanSum[0][0] / xPartition) / yPartition)* 100 ) / 100;
	std::cout << "|" << std::setw(10) << roundf(((nanSum[1][0] / xPartition) / yPartition)* 100 ) / 100;
	std::cout << "|" << std::setw(10) << roundf(((nanSum[2][0] / (xDim - 2 * xPartition)) / (yDim - 2 * yPartition)) * 100) / 100 << "|" << std::endl;

	std::cout << "|" << std::setw(10) << roundf(((nanSum[0][1] / xPartition) / yPartition) * 100) / 100;
	std::cout << "|" << std::setw(10) << roundf(((nanSum[1][1] / xPartition) / yPartition) * 100) / 100;
	std::cout << "|" << std::setw(10) << roundf(((nanSum[2][1] / (xDim - 2 * xPartition)) / (yDim - 2 * yPartition)) * 100) / 100 << "|" << std::endl;

	std::cout << "|" << std::setw(10) << roundf(((nanSum[0][2] / xPartition) / yPartition) * 100) / 100;
	std::cout << "|" << std::setw(10) << roundf(((nanSum[1][2] / xPartition) / yPartition) * 100) / 100;
	std::cout << "|" << std::setw(10) << roundf(((nanSum[2][2] / (xDim - 2 * xPartition)) / (yDim - 2 * yPartition)) * 100) / 100 << "|" << std::endl;

	std::vector<double> x,leftLeftPartition,middleLeftPartition,rightLeftPartition,leftRightPartition,middleRightPartition,rightRightPartition;
	for (int j = 0; j < 3; ++j) {
		for (int i = 0; i < yDim; ++i) {
			x.push_back((double)i);
			leftLeftPartition.push_back(dataMatrix[xPartition - 4 + j][i]);
			middleLeftPartition.push_back(dataMatrix[xPartition - 1 + j][i]);
			rightLeftPartition.push_back(dataMatrix[xPartition + 2 + j][i]);
			leftRightPartition.push_back(dataMatrix[2*xPartition - 4 + j][i]);
			middleRightPartition.push_back(dataMatrix[2*xPartition - 1 + j][i]);
			rightRightPartition.push_back(dataMatrix[2*xPartition + 2 + j][i]);
		}
	}

	std::cout << "slope 1: " << std::setw(10) << regressionSlope(x, leftLeftPartition) << std::endl;
	std::cout << "slope 2: " << std::setw(10) << regressionSlope(x, middleLeftPartition) << std::endl;
	std::cout << "slope 3: " << std::setw(10) << regressionSlope(x, rightLeftPartition) << std::endl;
	std::cout << "slope 4: " << std::setw(10) << regressionSlope(x, leftRightPartition) << std::endl;
	std::cout << "slope 5: " << std::setw(10) << regressionSlope(x, middleRightPartition) << std::endl;
	std::cout << "slope 6: " << std::setw(10) << regressionSlope(x, rightRightPartition) << std::endl;

	//resultOutput.close();
}

bool displayDebug;
std::string dataPath, tempPath, debugAnswer;
std::vector<std::string> x3pNames;

int main( int argc, char *argv[] )
{
	std::cout << "Please enter the file path to the data directory: " << std::endl;
	std::getline(std::cin, dataPath);
	std::cout << "Please enter the file path to the properly formatted temporary storage directory: " << std::endl;
	std::getline(std::cin, tempPath);
	std::cout << "Enter 'y' to see debug info, enter anything else otherwise" << std::endl;
	std::getline(std::cin, debugAnswer);
	displayDebug = (debugAnswer == "y");

	x3pNames = fileNames(dataPath);

	for (std::string path : x3pNames) {
		elz::extractZip(displayDebug, path, tempPath);
		processDataMatrix(tempPath);
		for (const auto & entry : fs::directory_iterator(tempPath)) {
			fs::remove_all(fs::path(entry));
		}
	}

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
