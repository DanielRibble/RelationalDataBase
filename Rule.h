#pragma once
#include "Predicate.h"
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Rule{
private:
	Predicate pred;
	vector<Predicate> pred_list;

public:
	Rule(){}
	~Rule(){}

	void setPred(Predicate pred){
		this->pred = pred;
	}

	Predicate getPredicate(){
		return pred;
	}

	void addPredList(Predicate pred_in){
		pred_list.push_back(pred_in);
	}

	vector<Predicate> getPredicateList(){
		return pred_list;
	}
	
	void clear(){
		pred_list.clear();
	}

	string toString(){
		string s;
		stringstream ss;
		ss << pred.toString() << " :- ";
		for (unsigned i = 0; i < pred_list.size(); i++){
			ss << pred_list[i].toString();
			if (i != pred_list.size()-1){
				ss << ",";
			}
		}

		s = ss.str();
		return s;
	}

};
