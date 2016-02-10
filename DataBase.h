#pragma once
#include "Relation.h"
#include <map>

using namespace std;

class DataBase{
private:
	map<string, Relation> DB;

public:
	DataBase(){}
	~DataBase(){}

	void addRelation(string name, Relation relation){
		DB[name] = relation;
	}

	Relation getRelation(string name){
		return DB[name];
	}

	void relationUnion(string name, set<Tuple> TtoAdd){
		set<Tuple>::iterator i;
		for (i = TtoAdd.begin(); i != TtoAdd.end(); i++){
			DB[name].addTuple(*i);	
		}
	}

	unsigned getTotalTupleCount(){
		map<string, Relation>::iterator it;
		unsigned total = 0;
		for (it = DB.begin(); it != DB.end(); it++){
			total += (it->second).getNumOfTuples();
		}
		return total;
	}
};
