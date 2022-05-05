#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
#include <map>
#include <cmath>
#include <bitset>
#include <ctime>

#include "component.h"
using namespace std;

TruthTable::TruthTable(const int in, const int out) {
	inputNum = in;
	outputNum = out;
	table = new list<list<bool>>();
	toTable(generate());
	permute(2);
}

TruthTable::TruthTable(list<string>* const tableString) {
	table = new list<list<bool>>();
	toTable(tableString);
}
void TruthTable::toTable(list<string>* const tableString) {
	string line;
	list<bool> linein;
	int inputlines = 0;
	for (auto& line : *tableString) {
		inputlines++;
		linein.clear();
		for (int i = 0; i < line.size(); i++) {
			if (line.at(i) == '0')
				linein.insert(linein.end(), false);
			else if (line.at(i) == '1')
				linein.insert(linein.end(), true);
			else cout << "Error loading " << inputlines << " line.\n";
		}
		table->insert(table->end(), linein);
	}
	inputNum = log2(++inputlines);
	outputNum = linein.size() - inputNum;
}

list<string>* TruthTable::generate() {
	//name = fmt::format("{:x}", 32);
	//name = std::format("{:x}", 32);
	//std::stringstream streamH;
	//streamH << std::hex << 32;

	int eint, inbound, outbound;
	string lineStr, buffer;
	list<string>* tableStr = new list<string>();
	
	inbound = pow(2, inputNum);
	outbound = pow(2, outputNum);
	for (int i=0; i<inbound; i++) {
		lineStr = "";
		eint = rand() % outbound;
		buffer = bitset<1024>(i).to_string();
		for (int j=0; j<inputNum; j++)
			lineStr += buffer.at(1024 - inputNum + j);
		buffer = bitset<1024>(eint).to_string();
		for (int j=0; j<outputNum; j++)
			lineStr += buffer.at(1024 - outputNum + j);
		tableStr->insert(tableStr->end(), lineStr);
	}
	return tableStr;
}
void TruthTable::permute(int num){
	int line1, line2;
	int inbound;
	inbound = pow(2, inputNum);
	//srand(time(NULL));

	for (int j = 0; j < num; j++) {
		line1 = rand() % inbound;
		do {
			line2 = rand() % inbound;
		} while (line1 == line2);
		auto itr1 = table->begin();
		for(int i=0; i<line1; i++)
			itr1++;
		auto itr2{table->begin()};
		for(int i=0; i<line2; i++)
			itr2++;
		(* itr1).swap(*itr2);
	}
}
