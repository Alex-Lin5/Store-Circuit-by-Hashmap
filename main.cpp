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
#include <map>
#include <cmath>
#include <bitset>
#include <ctime>
//#include <time.h>
//#include <fmt/core.h>

using namespace std;

class TruthTable {
private:
	list<list<bool>>* table;
	int inputNum;
	int outputNum;

	list<string>* generate();
	void permute(int num);
	void toTable(list<string>* const tableString);

public:
	TruthTable(){ table = new list<list<bool>>{}; inputNum = outputNum = 0;}
	TruthTable(list<string>* const tableString);
	TruthTable (const int in, const int out);
	~TruthTable() { delete table;}
	list<list<bool>>* getTable() { return table;}
	int getIn() { return inputNum; }
	int getOut() { return outputNum; }
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
	Circuit(const int in, const int out);
	Circuit(const string nameC, TruthTable* const &data);
	Circuit(ifstream& file);
	~Circuit(){ delete value;}
	string getName() { return name;}
	string getKey() { return key;}
	TruthTable* getValue() { return value;}
};

class Database {
private:
	unordered_map<string, Circuit*>* warehouse;
	//hashMap* warehouse;
	int circuitNum;

	bool checkNull(Circuit* const& C);
	bool checkExisted(Circuit* const& C);
	~Database();
public:
		Database(){
		circuitNum = 0;
		warehouse = new unordered_map<string, Circuit*>();
		// warehouse = new hashMap();
	}
	void Add(Circuit* const& C);
	void Find(Circuit* const& C);
	void Delete(Circuit*& const C);
	int getNum() { return circuitNum;}
	
};

typedef unordered_map<string, Circuit*> hashMap;

ostream& operator<<(ostream& str, Database& const DB);
ostream& operator<<(ostream& str, Circuit& const C);
ostream& operator<<(ostream& str, TruthTable& const T);
void test(Circuit* C);
int main(){
	srand(time(NULL));
	Database* DB1 = new Database();

	ifstream Cin("C:/MYFILE/CIS554/HW8_storeDLCby_hashmap/circuit.txt");

	Circuit* C1 = new Circuit(Cin);
	Circuit* C2 = new Circuit(Cin);
	Circuit* C3 = new Circuit(Cin);
	Circuit* R1 = new Circuit(3, 5);
	Circuit* R2 = new Circuit(2, 2);

	cout << *R1;
	cout << *R2;
	//test(C3);
	//cout << *C1;
	//cout << *C2;

	Cin.close();
	
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
void test(Circuit* C3) {
	delete C3;
	cout << *C3;

}
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

void Circuit::generateKey(){
	//00010
	//01000
	//01111
	//10000
	//00100
	//10111
	//11111
	//11001
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
	for (auto& line : *(value->getTable())) {
			int i=0;
		inputCount = 0;
		outputCount = 0;
		for (auto& e: line) {
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
	string name;
	getline(file, line);
	name = line;
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
	this->name = name;
	return tableStr;
}
Circuit::Circuit(const int in, const int out) {
	//srand(time(NULL));
	name = 'R' + to_string(rand() % 9000 + 1000); // 1000-9999
	value = new TruthTable(in, out);
	generateKey();
}

Circuit::Circuit() {
	name = "";
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
ostream& operator<<(ostream& str, TruthTable& const T){
	for (auto& L: *(T.getTable())) {
		for (auto& i: L)
			str << i;
		str << '\n';
	}
	return str;
}

ostream& operator<<(ostream& str, Circuit& const C){
	if (&C == nullptr) return str;
	str << C.getName() << endl;
	str << C.getValue()->getIn() << '\n' << C.getValue()->getOut() << '\n';
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
	}
	circuitNum--;

	cout << endl;
}
//// You need to implement overloaded operator<< to facilitate printing a circuit (See the possible
//// output messages below.)
//// Input file “input.txt”, with the following format.
//// 25//number of database operations, such as Add, Find, or Delete
//// Add
//// Circuit_1
//// 3//number of inputs
//// 2//number of outputs
//// 000 10
//// 001 00
//// 010 00
//// 011 10
//// 100 00
//// 101 11
//// 110 01
//// 111 11
//// Add
//// Circuit_2
//// 4
//// 2
//// 0000 01
//// 0001 11
//// 0010 00
//// …
//// Find
//// Circuit_3
//// ….
//// Delete
//// Circuit_8
//// …
//// Add
//// Circuit 20
//// …
//// Possible output messages for “add” are in the following:
//// Circuit 4 is added to the database.
//// Circuit 4 is not added. It has a re-useable circuit, Circuit 1, in the database, whose truth table is
//// shown below.
//// Circuit_1
//// 3
//// 2
//// 000 10
//// 001 00
//// 010 00
//// 011 10
//// 100 00
//// 101 11
//// 110 01
//// 111 11
//// Possible output messages for “find” are in the following:
//// Circuit 4 does not have any re-useable circuit in the database.
//// Circuit 4 has a re-useable circuit, Circuit 1, in the database, whose truth table is shown below.
//// Circuit_1
//// 3
//// 2
//// 000 10
//// 001 00
//// 010 00
//// 011 10
//// 100 00
//// 101 11
//// 110 01
//// 111 11
//// Possible output messages for “delete” are in the following.
//// Circuit 4 does not have any re-useable circuit in the database.
//// Circuit 4 has a re-useable circuit, Circuit 1, in the database, whose truth table is shown below
//// and Circuit 1 is deleted from the database.
//// Circuit_1
//// 3
//// 2
//// 000 10
//// 001 00
//// 010 00
//// 011 10
//// 100 00
//// 101 11
//// 110 01
//// 111 11