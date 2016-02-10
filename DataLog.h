#pragma once
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
#include <vector>
#include <string>
#include <set>

using namespace std;

class DataLog
{
private:
	vector<Predicate> schemes;
	vector<Predicate> facts;
	vector<Rule> rules;
	vector<Predicate> queries;
	set<string> domains;  //or store STRING token

public:
	DataLog(){}
	~DataLog(){}

	void addScheme(Predicate scheme){
		schemes.push_back(scheme);
	}

	vector<Predicate> getSchemes(){
		return schemes;
	}

	void addFact(Predicate fact){
		facts.push_back(fact);
	}

	vector<Predicate> getFacts(){
		return facts;
	}

	void addRule(Rule rule){
		rules.push_back(rule);
	}

	vector<Rule> getRules(){
		return rules;
	}

	void addQuery(Predicate query){
		queries.push_back(query);
	}

	vector<Predicate> getQueries(){
		return queries;
	}

	void addDomain(string domain){
		domains.insert(domain);
	}

	string toString(){
		string s;
		stringstream ss;
		ss << "Schemes(" << schemes.size() << "):" << endl;
		for (unsigned i = 0; i < schemes.size(); i++){
			ss << "  " << schemes[i].toString() << endl;
		}
		ss << "Facts(" << facts.size() << "):" << endl;
		for (unsigned i = 0; i < facts.size(); i++){
			ss << "  " << facts[i].toString() << endl;
		}
		ss << "Rules(" << rules.size() << "):" << endl;
		for (unsigned i = 0; i < rules.size(); i++){
			ss << "  " << rules[i].toString() << endl;
		}
		ss << "Queries(" << queries.size() << "):" << endl;
		for (unsigned i = 0; i < queries.size(); i++){
			ss << "  " << queries[i].toString() << endl;
		}
		ss << "Domain(" << domains.size() << "):" << endl;
		set<string>::iterator j; 
		for (j = domains.begin(); j != domains.end(); j++){
			ss << "  '" << *j <<  "'" << endl;
		}

		s = ss.str();
		return s;
	}

};


