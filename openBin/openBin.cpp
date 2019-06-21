// openBin.cpp : This file contains the 'main' function. Program execution begins and ends there.
// test

#include "pch.h" //why is this here
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
int main()
{
	int xDim = 3671, yDim = 1023;
	std::string inputTxt = "C:/Users/tji/source/repos/openBin/openBin/data.bin";
	std::string outputTxt = "C:/Users/tji/source/repos/openBin/openBin/numbers.txt";

	std::vector<std::vector<double> > dataMatrix;
	double data;
	std::ifstream input;
	std::ofstream output;

	input.open(inputTxt, std::ios::binary); // reads binary file and converts binary (ios::binary, overloads)
	output.open(outputTxt); // opening what we're writing to

	//include bool checks on file in separate method
	//iterators
	for(int i = 0; i < yDim; ++i) { //while we are not at the end of the file
		std::vector<double> dataRow;
		dataMatrix.push_back(dataRow);
		for (int j = 0; j < xDim; ++j) {
			input.read((char*)&data, sizeof(data)); // read next line into "data"
			dataMatrix[i].push_back(data);
			std::cout << std::setw(10) << dataMatrix[i][j];
			output << std::setw(10) << dataMatrix[i][j];
		}
		std::cout << std::endl;
		output << std::endl;
	}
	input.close(); //close input file
	output.close();// close output file 



	//test

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
