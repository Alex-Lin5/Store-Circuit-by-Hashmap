// CIS554 HW8. Due: Thursday, 11:59pm, April 28, 2022. (This is more a take-home exam than
// regular HW. Discussions with classmates are limited to clarifications.)
// HW Objective: Store combinational logic circuits with one or more outputs to a database,
// implemented using unordered_map. Each element of the unordered_map is {Circuit, pointer to
// circuit name}.
// (In actual design environment, it should be a pointer to circuit implementation, rather than
// circuit name as is done here.) More explanation will be given in class.
// As discussed in class, permuting inputs columns, output columns, or rows of a circuit’s truth
// table will lead to different circuits which can re-use the original circuit and thus do not need to
// be re-designed. Thus, we should only keep one circuit in the database for all of them.
// You need to be able to Add, Delete, and Find a circuit.
// You will design the data structures and functions.
// You need to design a class, Circuit.
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
//#include <initializer_list>
#include <map>
#include <cmath>
#include <bitset>
#include <ctime>
//#include <time.h>
//#include <fmt/core.h>

//#include "component.h"

using namespace std;

class Node {
private:
	shared_ptr<Node> right;
	weak_ptr<Node> down;
	bool value;
public:
	Node(){ value = NULL;}
	Node(bool data){ value = data; right.reset(); down.reset(); }
	//~Node(){ value = NULL;}
	bool get(){ return value;}
	void setR(shared_ptr<Node> theNode) { this->right = theNode; }
	void setD(shared_ptr<Node> theNode) { this->down = theNode; }
	shared_ptr<Node> getR() { return right; }
	shared_ptr<Node> getD() { return down.lock(); }
};

//template<class T>
class TruthTable {
private:
	shared_ptr<Node> entry;
	int inputNum;
	int outputNum;

	list<string>* generate();
	void permuteRow();
	void permuteCol();
	void permute(int num);
	void toTable(list<string>* const tableString);
	template<class T> bool check(const T value);
	template<typename T> void initialize(T& const itrB, const T& itrE);

public:
	TruthTable();
	TruthTable(const initializer_list<int>& L);
	TruthTable(list<string>* const tableString);
	TruthTable (const int in, const int out);
	~TruthTable() { entry.reset(); }
	shared_ptr<Node> getEntry() { return entry;}
	int getIn() { return inputNum; }
	int getOut() { return outputNum; }
	vector<bool>* getRow(const int num);
	vector<bool>* getCol(const int num);
	//bool get(int row, int col);
};

class Circuit {
private:
	string name;
	string key;
	TruthTable* value;

	list<string>* fetchFile(ifstream& file);
	void Add (TruthTable* const data) { value = data;}
	void Add (list<string>* const tableString) {
		value = new TruthTable(tableString);
	}
	void generateKey();
public:	
	Circuit();
	Circuit(const initializer_list<int>& L);
	Circuit(const int in, const int out);
	Circuit(const string nameC, TruthTable* const &data);
	Circuit(ifstream& file);
	~Circuit(){ delete value;}
	string getName() { return name;}
	string getKey() { return key;}
	TruthTable* getValue() { return value;}
};

typedef unordered_map<string, Circuit*> hashMap;

class Database {
private:
	hashMap* warehouse;
	int circuitNum;

	bool checkNull(Circuit* const& C);
	bool checkExisted(Circuit* const& C);
	~Database();
public:
		Database(){
		circuitNum = 0;
		warehouse = new hashMap();
	}
	void Add(Circuit* const& C);
	void Find(Circuit* const& C);
	void Delete(Circuit*& const C);
	int getNum() { return circuitNum;}
	
};


ostream& operator<<(ostream& str, Database& const DB);
ostream& operator<<(ostream& str, Circuit& const C);
ostream& operator<<(ostream& str, TruthTable& const T);
void test();
int main(){
	srand(time(NULL));
	Database* DB1 = new Database();

	ifstream Cin("C:/MYFILE/StoreCircuits_byHashmap/circuit.txt");

	//Circuit* C1 = new Circuit(Cin);
	//Circuit* C2 = new Circuit(Cin);
	//Circuit* C3 = new Circuit(Cin);
	//Circuit* R1 = new Circuit(3, 5);
	//Circuit* R2 = new Circuit(2, 2);

	//cout << *R1;
	//cout << *R2;
	//cout << *C1;
	//cout << *C2;
	Cin.close();

	test();
	
	//DB1->Add(R1);
	//DB1->Delete(R1);
	//DB1->Add(R1);
	//DB1->Add(C1);
	//DB1->Add(C1);
	//DB1->Add(C2); // C1 == C3
	//DB1->Find(C1);
	//DB1->Find(C2);
	//DB1->Delete(C3);
	//DB1->Delete(C1);
	//cout << *DB1;
	//delete DB1;

	ofstream dataout("database.txt");
	dataout.close();
	return 0;
}
void test() {
	//Circuit* C3 = new Circuit(3, 5);
	//delete C3;
	//cout << *C3;
	auto data = {
		2, 1,
		0, 0, 1,
		0, 1, 0,
		1, 0, 0,
		1, 1, 1	
	};
	Circuit* I1 = new Circuit(data);
	cout << *I1;

	Circuit* R1 = new Circuit(2, 8);
	cout << *R1;


}
TruthTable::TruthTable(){ 
	//table = new list<list<bool>>{}; 
	entry = make_shared<Node>();
	inputNum = outputNum = 0; 
	cout << "Default constructor\n";
}

TruthTable::TruthTable(const int in, const int out) {
	inputNum = in;
	outputNum = out;
	//table = new list<list<bool>>();
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
	//table = new list<list<bool>>();
	toTable(tableString);
}

template<typename T> void TruthTable::initialize(T& const itrB, const T& itrE) {
	int index{ 0 };
	int lineWidth{ inputNum + outputNum };
	//auto itr{L.begin};
	shared_ptr<Node> pre{ nullptr }, above{ nullptr };
	//"0000 1010 1110"
	while (itrB <= itrE) {
		//while (itrB != L.end()) {
		shared_ptr<Node> node = make_shared<Node>(check(*itrB));
		itrB++;
		//if (itrB == itrE) ;
		//else itrB++;
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
vector<bool>* TruthTable::getRow(const int num) {
	vector<bool>* row = new vector<bool>();
	if ((num >= 0) && num < (pow(2, inputNum))) {
		auto pt{ entry };
		for (int i = 0; i < num; i++)
			pt->getD();
		for (int i = 0; i < inputNum + outputNum; i++) {
			row->push_back(pt->get());
			pt = pt->getR();
		}
	}
	else
		cout << "Input out of bound\n";
	return row;
}

vector<bool>* TruthTable::getCol(const int num) {
	vector<bool>* col = new vector<bool>();
	if ((num >= 0) && num < (inputNum + outputNum)) {
		auto pt{ entry };
		for (int i = 0; i < num; i++)
			pt->getR();
		for (int i = 0; i < pow(2, inputNum); i++) {
			col->push_back(pt->get());
			pt->getD();
		}
	}
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
		auto line = *(value->getRow(j));
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
	name = "default name";
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

//// Your program will read in circuits and the operations from the file “input.txt”.
//// You will implement the following three functions:
 
ostream& operator<<(ostream& str,  Database& const DB){
	str << "Database contains " << DB.getNum() << " circuits.\n";
	return str;
}

ostream& operator<<(ostream& str, TruthTable& const T) {
	int inNum{T.getIn()};
	int outNum{T.getOut()};
	int lineWidth{inNum + outNum};
	auto pt{T.getEntry()};

	int index{1};
	str << inNum << '\n' << outNum << '\n';
	while (pt->getR() != nullptr) {
		str << pt->get();
		int rem{index % lineWidth};
		if (rem == inNum)
			str << " ";
		else if (rem == 0)
			str << "\n";
		index++;
		pt = pt->getR();
	}
	return str;
}

ostream& operator<<(ostream& str, Circuit& const C) {
	if (&C == nullptr) return str;
	str << C.getName() << endl;
	str << *(C.getValue());
	return str;
}

Database::~Database(){
	if (warehouse->empty()) return;
	auto itr{ warehouse->begin() };
	while (itr != warehouse->end()) {
		delete itr->second;
		circuitNum--;
		auto tp{itr};
		itr++;
		warehouse->erase(tp);
	}
}
bool Database::checkNull(Circuit* const& C) {
	if (C == nullptr){
		std::cout << "Circuit does not existed on memory.\n";
		return true;
	} 
	return false;
}
bool Database::checkExisted(Circuit* const& C) {
	bool existed;
	if (warehouse->find(C->getKey()) != warehouse->end())
		existed = true;
	else
		existed = false;
	return existed;
}
void Database::Add(Circuit* const& C) {
// void Database::Add(hashMap& DB, const Circuit& C) {
// Add
// Circuit_2
// 4
// 2
// 0000 01
// 0001 11
// 0010 00
// Circuit 4 is added to the database.
// Circuit 4 is not added. It has a re-useable circuit, Circuit 1, in the database, whose truth table is

	cout << "Add\n";
	if (checkNull(C)) return;
	cout << *C;

	if (!checkExisted(C)){
		warehouse->insert(make_pair(C->getKey(), C));
		cout << "Circuit " << C->getName() << " is added to the database.\n";
		circuitNum++;
	}
	else{
		cout << "Circuit " << C->getName() << " is not added.It has a re-useable circuit,"
		<< " in the database, whose truth table is shown below.\n";
		cout << *((*warehouse)[C->getKey()]->getValue());
		//delete C;
	}
	cout << endl;
}
void Database::Find(Circuit* const& C){
// void Database::Find(hashMap& DB, const Circuit& C){
// Find
// Circuit_3
// Circuit 4 does not have any re-useable circuit in the database.
// Circuit 4 has a re-useable circuit, Circuit 1, in the database, whose truth table is shown below.
	
	cout << "Find\n";
	if (checkNull(C)) return;
	cout << *C;

	if (!checkExisted(C)) 
		cout << "Circuit " << C->getName() << " does not have any re-useable circuit in the database.\n";
	else{
		cout << "Circuit " << C->getName() << " has a re-useable circuit,"
		<< " in the database, whose truth table is shown below.\n";
		cout << *((*warehouse)[C->getKey()]->getValue());
	}
	cout << endl;
}
void Database::Delete(Circuit*& const C) {
	// Delete
	// Circuit_8
	// Circuit 4 does not have any re-useable circuit in the database.
	// Circuit 4 has a re-useable circuit, Circuit 1, in the database, whose truth table is shown below
	// and Circuit 1 is deleted from the database.

	cout << "Delete\n";
	if (checkNull(C)) return;
	cout << *C;

	auto itr{ warehouse->find(C->getKey()) };
	if (!checkExisted(C)) {
		cout << "Circuit " << C->getName() << " does not have any re-useable circuit in the database.\n";
	}
	else {
		cout << "Circuit " << C->getName() << " has a re-useable circuit,"
		<< " in the database, whose truth table is shown below.\n";
		cout << *((*warehouse)[C->getKey()]->getValue());
		warehouse->erase(itr);
		cout << "Circuit " << C->getName() << " is deleted from the database.\n";
		delete C;
		C = nullptr;
		circuitNum--;
	}

	cout << endl;
}