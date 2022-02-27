#include "json.hpp"
#include <iostream>
#include <fstream>

// for convenience
using json = nlohmann::json;
using std::ifstream;
using std::ofstream;


void main() {
	// read a JSON file
	ifstream inputFile("input.json");
	json inputJSON;
	inputFile >> inputJSON;

	ifstream inputFileNew("output.json");
	json outputJSON;
	inputFileNew >> outputJSON;


	std::ofstream o("pretty.json");
	o << std::setw(4) << inputJSON << std::endl;
} 