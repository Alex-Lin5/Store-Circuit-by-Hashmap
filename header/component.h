#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
#include <map>
#include <cmath>
#include <bitset>
#include <ctime>

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
	TruthTable(const TruthTable& T);
	void operator=(const TruthTable& T);

	shared_ptr<Node> getEntry() { return entry;}
	int getIn() { return inputNum; }
	int getOut() { return outputNum; }
	vector<bool> getRow(const int num);
	vector<bool> getCol(const int num);
	//void setEntry(shared_ptr<Node> node) { entry = node;}
	//bool get(int row, int col);
};

