#pragma once
#include "Parameter.h"
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Predicate
{
private:
	string identifier;
	vector<Parameter> parameter_list;

public:
	Predicate(){}
	~Predicate(){}

	void setID(string id_in){
		identifier = id_in;
	}

	string getID(){
		return identifier;
	}

	void addParam(Parameter param){
		parameter_list.push_back(param);
	}

	vector<Parameter> getParameterList(){
		return parameter_list;
	}

	void clear(){
		identifier = "";
		parameter_list.clear();
	}

	string toString(){
		string s;
		stringstream ss;
		ss << identifier << "(";
		for (unsigned i = 0; i < parameter_list.size(); i++){
			ss << parameter_list[i].toString();
			if (i != parameter_list.size()-1){
				ss << ",";
			}
		}
		ss << ")";
		
		s = ss.str();
		return s;
	}
};