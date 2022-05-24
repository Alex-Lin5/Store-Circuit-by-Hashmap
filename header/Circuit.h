#include "TruthTable.h"

#pragma once

#ifndef CIRCUIT_H
#define CIRCUIT_H
class Circuit {
private:
	string name;
	string key;
	TruthTable* value;

	list<string>* fetchFile(ifstream& file);
	void Add(TruthTable* const data) { value = data; }
	void Add(list<string>* const tableString) {
		value = new TruthTable(tableString);
	}
	void generateKey();
public:
	Circuit();
	Circuit(const initializer_list<int>& L);
	Circuit(const int in, const int out);
	Circuit(const string nameC, TruthTable* const& data);
	Circuit(ifstream& file);
	~Circuit() { delete value; }
	Circuit(const Circuit& C);
	void operator=(const Circuit& C);
	Circuit(Circuit&& C);
	void operator=(Circuit&& C);

	Circuit invert();
	template<class Comp> void sort(Comp functor);
	string getName() { return name; }
	string getKey() { return key; }
	TruthTable* getValue() { return value; }
	friend ostream& operator<<(ostream& str, Circuit& const C);
};


#endif // !CIRCUIT_H

