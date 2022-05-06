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

TruthTable::TruthTable(){ 
	entry = make_shared<Node>();
	inputNum = outputNum = 0; 
	cout << "Default constructor\n";
}

TruthTable::TruthTable(const int in, const int out) {
	inputNum = in;
	outputNum = out;
	toTable(generate());
	//permute(2);
}

TruthTable::TruthTable(const initializer_list<int>& L) {
	if (L.size() < 1){
		TruthTable();
		return;
	} 
	auto itr{L.begin()};
	inputNum = *itr;
	outputNum = *(++itr);
	initialize(++itr, L.end());
}

TruthTable::TruthTable(list<string>* const tableString) {
	toTable(tableString);
}
void TruthTable::operator=(const TruthTable& T) {
	//*this = new TruthTable(T);
	//TruthTable(T);
	inputNum = T.inputNum;
	outputNum = T.outputNum;
	vector<bool> table;
	auto pt{ T.entry };
	while (pt != nullptr) {
		table.push_back(pt->get());
		pt = pt->getR();
	}
	auto itr1{ table.begin() };
	auto itr2{ table.end() };
	initialize(itr1, --itr2);
}

TruthTable::TruthTable(const TruthTable& T) {
	inputNum = T.inputNum;
	outputNum = T.outputNum;
	vector<bool> table;
	auto pt{T.entry};
	while (pt != nullptr) {
		table.push_back(pt->get());
		pt = pt->getR();
	}
	auto itr1{ table.begin() };
	auto itr2{ table.end() };
	initialize(itr1, --itr2);
}

template<typename T> void TruthTable::initialize(T& const itrB, const T& itrE) {
	int index{ 0 };
	int lineWidth{ inputNum + outputNum };
	shared_ptr<Node> pre{ nullptr }, above{ nullptr };
	while (itrB <= itrE) {
		shared_ptr<Node> node = make_shared<Node>(check(*itrB));
		itrB++;
		//shared_ptr<Node> node = make_shared<Node>(check(*(itrB++)));
		if (index == 0) {
			entry = node;
			pre = node;
			above = node;
			index++;
			continue;
		}
		else if (index < lineWidth);
		else if (index >= lineWidth) {
			above->setD(node);
			above = above->getR();
		}
		pre->setR(node);
		pre = node;
		index++;
	}
	pre->setR(nullptr);

}
vector<bool> TruthTable::getRow(const int num) {
	vector<bool> row;
	//auto power = [&, inputNum](int p) { for (int i = 0; i < p; i++) result *= inputNum; };
	auto power2 = [&]() { int result{1}; 
	for (int i = 0; i < inputNum; i++) result *= 2;
	return result;};
	if (num >= 0 && num < power2()) {
			auto pt{ entry };
		for (int i = 0; i < num; i++)
			pt->getD();
		for (int i = 0; i < inputNum + outputNum; i++) {
			row.push_back(pt->get());
			pt = pt->getR();
		}
	}
	else
		cout << "Input of row out of bound\n";
	return row;
}

vector<bool> TruthTable::getCol(const int num) {
	//vector<bool>* col = new vector<bool>();
	vector<bool> col;
	int result{1};
	auto power = [&result](int base, int power) { 
		for (int i = 0; i < power; i++) return result *= base; };

	if ((num >= 0) && num < (inputNum + outputNum)) {
		auto pt{ entry };
		for (int i = 0; i < num; i++)
			pt->getR();
		for (int i = 0; i < power(2, inputNum); i++) {
			//for (int i = 0; i < pow(2, inputNum); i++) {
				col.push_back(pt->get());
			pt->getD();
		}
	}
	else
		cout << "Input of column out of bound\n";
	return col;
}

template<class T> bool TruthTable::check(const T value) {
	if (value == 1 || value == '1')
		return true;
	else if (value == 0 || value == '0')
		return false;
	else return NULL;
}

void TruthTable::toTable(list<string>* const tableString) {
	inputNum = log2(tableString->size());
	outputNum = (*tableString->begin()).size() - inputNum;

	string whole{""};
	auto itr{tableString->begin()};
	auto itrE{tableString->end()};
	while (itr != itrE) {
		whole += *(itr++);
	}
	whole += " ";
	auto itr1{ whole.begin() };
	auto itr2{ whole.end() };
	initialize(itr1, --itr2);
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
	for (int i = 0; i < inbound; i++) {
		lineStr = "";
		eint = rand() % outbound;
		buffer = bitset<1024>(i).to_string();
		for (int j = 0; j < inputNum; j++)
			lineStr += buffer.at(1024 - inputNum + j);
		buffer = bitset<1024>(eint).to_string();
		for (int j = 0; j < outputNum; j++)
			lineStr += buffer.at(1024 - outputNum + j);
		tableStr->insert(tableStr->end(), lineStr);
	}
	return tableStr;
}
void TruthTable::permute(int num) {
	int line1, line2;
	int inbound;
	inbound = pow(2, inputNum);

	//for (int j = 0; j < num; j++) {
	//	line1 = rand() % inbound;
	//	do {
	//		line2 = rand() % inbound;
	//	} while (line1 == line2);
	//	auto itr1 = table->begin();
	//	for (int i = 0; i < line1; i++)
	//		itr1++;
	//	auto itr2{ table->begin() };
	//	for (int i = 0; i < line2; i++)
	//		itr2++;
	//	(*itr1).swap(*itr2);
	//}
}
