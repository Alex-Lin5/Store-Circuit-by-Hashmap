#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
#include <map>
#include <cmath>
#include <bitset>
#include <ctime>
//#include <fmt/core.h>

#include "Circuit.h"
#include "TruthTable.h"
#include "Database.h"
using namespace std;

void test();
int main(){
	srand(time(NULL));
	Database* DB1 = new Database();

	//test();
	ifstream Cin("C:/MYFILE/StoreCircuits_byHashmap/circuit.txt");

	Circuit* C1 = new Circuit(Cin);
	Circuit* C2 = new Circuit(Cin);
	Circuit* C3 = new Circuit(Cin);
	Circuit* R1 = new Circuit(3, 5);
	Circuit* R2 = new Circuit(2, 2);

	cout << *R1;
	cout << *R2;
	cout << *C1;
	cout << *C2;
	Cin.close();

	
	DB1->Add(R1);
	DB1->Delete(R1);
	DB1->Add(R1);
	DB1->Add(C1);
	DB1->Add(C1);
	DB1->Add(C2); // C1 == C3
	DB1->Find(C1);
	DB1->Find(C2);
	DB1->Delete(C3);
	DB1->Delete(C1);
	cout << *DB1;
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
	Circuit C1;
	C1 = *I1;
	cout << C1;
	Circuit* C2 = new Circuit(*I1);
	cout << *C2;

	Circuit M1;
	M1 = I1->invert();
	cout << M1;

	Circuit* R1 = new Circuit(2, 8);
	cout << *R1;
	Circuit* D1 = new Circuit();
	cout << *D1;


}
 
