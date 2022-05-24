#include "Database.h"

ostream& operator<<(ostream& str, Database& const DB) {
	str << "Database contains " << DB.getNum() << " circuits.\n";
	return str;
}



Database::~Database() {
	if (warehouse->empty()) return;
	auto itr{ warehouse->begin() };
	while (itr != warehouse->end()) {
		delete itr->second;
		circuitNum--;
		auto tp{ itr };
		itr++;
		warehouse->erase(tp);
	}
}
bool Database::checkNull(Circuit* const& C) {
	if (C == nullptr) {
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

	if (!checkExisted(C)) {
		warehouse->insert(make_pair(C->getKey(), C));
		cout << "Circuit " << C->getName() << " is added to the database.\n";
		circuitNum++;
	}
	else {
		cout << "Circuit " << C->getName() << " is not added.It has a re-useable circuit,"
			<< " in the database, whose truth table is shown below.\n";
		cout << *((*warehouse)[C->getKey()]->getValue());
		//delete C;
	}
	cout << endl;
}
void Database::Find(Circuit* const& C) {
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
	else {
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