#include "json.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <utility>

// for convenience
using json = nlohmann::json;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::to_string;


void main() {
	// read a JSON file
	ifstream inputFile("SystemMessageTranslated.json"); //Already translated/old JSON file
	json inputJSON;
	map<string, pair<string, string>> inputMap; //key = name of asset	pair.first = japanese text, pair.second = english text
	inputFile >> inputJSON;

	ifstream inputFileNew("SystemMessageNew.json"); //JSON from new version that is not translated
	json outputJSON;
	inputFileNew >> outputJSON;

	ofstream changes;
	changes.open("changes.txt"); //Any assets with japanese that has been changed between versions

	//Store Input JSON into map
	for (const auto& exports : inputJSON["Exports"]) {
		if (exports.find("Table") != exports.end() and
			exports["Table"].find("Data") != exports["Table"].end())
		{
			for (const auto& dataTable : exports["Table"]["Data"])
			{
				cout << dataTable["Name"] << endl;
				if (dataTable.find("Value") != dataTable.end())
				{
					for (const auto& textValue : dataTable["Value"])
					{
						if (textValue.is_object()) {
							if (textValue["Name"] == "JapaneseMessage(0)") {
								cout << textValue["Value"] << endl;
								if (textValue["Value"].is_string()) {
									inputMap[dataTable["Name"]].first = textValue["Value"];
								}
							}
							if (textValue["Name"] == "EnglishMessageUSA(0)") {
								cout << textValue["Value"] << endl;
								if (textValue["Value"].is_string()) {
									inputMap[dataTable["Name"]].second = textValue["Value"];
								}
							}
						}
					}
				}
			}
		}
	}

	//Compare output JSON with input JSON using the stored map
	for (auto& exports : outputJSON["Exports"]) {
		if (exports.find("Table") != exports.end() and
			exports["Table"].find("Data") != exports["Table"].end())
		{
			for (auto& dataTable : exports["Table"]["Data"])
			{
				if (dataTable.find("Value") != dataTable.end())
				{
					for (auto& textValue : dataTable["Value"])
					{
						if (textValue.is_object()) {
							if (textValue["Name"] == "JapaneseMessage(0)") {
								if (inputMap[dataTable["Name"]].first != textValue["Value"]) {
									changes << dataTable["Name"] << endl;
								}
							}
							if (textValue["Name"]  == "EnglishMessageUSA(0)") {
								if (inputMap[dataTable["Name"]].second != "") {
									textValue["Value"] = inputMap[dataTable["Name"]].second;
								}
								cout << textValue["Value"] << endl;
							}
						}
					}
				}
			}
		}
	}


	ofstream output("SystemMessageNew.json");
	output << std::setw(2) << outputJSON << endl;
} 