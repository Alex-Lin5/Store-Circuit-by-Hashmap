#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
#include <map>
#include <cmath>
#include <bitset>
#include <ctime>

class TruthTable {
private:
	list<list<bool>>* table;
	int inputNum;
	int outputNum;

	list<string>* generate();
	void permuteRow();
	void permuteCol();
	void permute(int num);
	void toTable(list<string>* const tableString);

public:
	TruthTable();
	TruthTable(list<string>* const tableString);
	TruthTable (const int in, const int out);
	~TruthTable();
	list<list<bool>>* getTable();
	int getIn();
	int getOut() { return outputNum; }
	vector<bool>* getRow();
	vector<bool>* getCol();
	bool get(int row, int col);
};
