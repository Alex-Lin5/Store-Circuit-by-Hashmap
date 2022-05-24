#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
#include <map>
#include <cmath>
#include <bitset>

#include "Circuit.h"
#include "TruthTable.h"

typedef unordered_map<string, Circuit*> hashMap;

class Database {
private:
	hashMap* warehouse;
	int circuitNum;

	bool checkNull(Circuit* const& C);
	bool checkExisted(Circuit* const& C);
	~Database();
public:
	Database() {
		circuitNum = 0;
		warehouse = new hashMap();
	}
	void Add(Circuit* const& C);
	void Find(Circuit* const& C);
	void Delete(Circuit*& const C);
	int getNum() { return circuitNum; }
	friend ostream& operator<<(ostream& str, Database& const DB);
};
