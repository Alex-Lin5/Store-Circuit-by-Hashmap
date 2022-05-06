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

Circuit::Circuit(const Circuit& C) {
	if (&C != this) {
		if (this->value != nullptr)
			delete value;
		name = "CC" + C.name;
		key = C.key;
		value = new TruthTable(*(C.value));
	}
	else
		cout << "construct by itself\n";
	cout << "copy constrctor\n";
}
void Circuit::operator=(const Circuit& C) {
	if (&C != this) {
		if (this->value != nullptr)
			delete value;
		name = "CA" + C.name;
		key = C.key;
		value = new TruthTable(*(C.value));
	}
	else
		cout << "Assign to itself\n";
	cout << "copy assignment\n";
}
Circuit::Circuit(Circuit&& C) {
	if (&C != this) {
		if (this->value != nullptr) {
			delete value;
		}
		name = "MC" + C.name;
		key = C.key;
		value = C.value;
		C.value = nullptr;
	}
	else
		cout << "construct by itself\n";
	cout << "move constructor\n";
}
void Circuit::operator=(Circuit&& C) {
	if (&C != this) {
		if (this->value != nullptr) {
			delete value;
		}
		name = "MA" + C.name;
		key = C.key;
		value = C.value;
		C.value = nullptr;
	}
	else
		cout << "Assign to itself\n";
	cout << "move assignment\n";

}
Circuit Circuit::invert() {
	Circuit tp{*this};
	return move(tp);
}

void Circuit::generateKey(){
	string object{""};
	map<int, map<int, int>> keyMap;
	// initialize with 0s
	for (int i=0; i<=value->getIn(); i++) {
		for (int j=0; j<=value->getOut(); j++) {
			keyMap[i].insert({j, 0});
		}
	}
	// counting 1s
	int inputCount{0}, outputCount{0}, repetition;
	for (int j=0; j<pow(2, value->getIn()); j++) {
		auto line = value->getRow(j);
		int i = 0;
		inputCount = 0;
		outputCount = 0;
		for (auto e: line) {
			if (i<value->getIn()) {
				if (e == true) inputCount++;
			}
			else {
				if (e == true) outputCount++;
			}
			i++;
		}
		repetition = keyMap[inputCount][outputCount];
		keyMap[inputCount].erase(outputCount);
		keyMap[inputCount].insert({outputCount, ++repetition});

	}
	// convert to plain string
	for (auto& in: keyMap) {
		object += to_string(in.first);
		for (auto& rep: in.second) {
			object += to_string(rep.first);
			object += to_string(rep.second);
		}
	}
	key = object;
}
list<string>* Circuit::fetchFile(ifstream& file) {
	string line;
	list<bool> linein;
	list<string>* tableStr = new list<string>();
	int inputN, outputN, count;
	string nameN;
	getline(file, line);
	nameN = line;
	getline(file, line);
	inputN = stoi(line);
	getline(file, line);
	outputN = stoi(line);

	count = 0;
	while (getline(file, line)) {
		if (count >= pow(2, inputN)) break;
		tableStr->insert(tableStr->end(), line);
		count++;
	}
	name = 'F' + to_string(rand() % 9000 + 1000); // 1000-9999
	return tableStr;
}
Circuit::Circuit(const initializer_list<int>& L) {
	name = 'I' + to_string(rand() % 9000 + 1000); // 1000-9999
	value = new TruthTable(L);
	generateKey();
}

Circuit::Circuit(const int in, const int out) {
	name = 'R' + to_string(rand() % 9000 + 1000); // 1000-9999
	value = new TruthTable(in, out);
	generateKey();
}

Circuit::Circuit() {
	name = "D0000";
	key = "";
	value = new TruthTable();
}

Circuit::Circuit(const string nameC, TruthTable* const &data){
	name = nameC;
	value = data;
	generateKey();
}
Circuit::Circuit(ifstream& file){
	if (!file.is_open()) {
		cout << "File is not open" << endl;
		Circuit();
		return;
	}
	list<string>* temp = fetchFile(file);
	Add(temp);
	generateKey();
}
