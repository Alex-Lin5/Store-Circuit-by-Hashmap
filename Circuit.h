#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
#include <map>
#include <cmath>
#include <bitset>
#include <ctime>


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
	Circuit(const Circuit& C);
	void operator=(const Circuit& C);
	Circuit(Circuit &&C);
	void operator=(Circuit &&C);

	Circuit invert();
	template<class Comp> void sort(Comp functor);
	string getName() { return name;}
	string getKey() { return key;}
	TruthTable* getValue() { return value;}
};
