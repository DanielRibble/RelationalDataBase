#pragma once
#include "Scanner.h"
#include "DataLog.h"

using namespace std;

class Parser
{
private:
	vector<Token> all_tokens;
	unsigned position;
	Predicate P;
	Rule R;
	Parameter Pa;
	DataLog DL;

public:
	Parser(vector<Token> all_tokens){
		position = 0;
		this->all_tokens = all_tokens;
	}
	~Parser(){}

	DataLog getDataLog(){
		return DL;
	}

//START PARSING
	bool parse(/*ofstream& write*/){
		try{
			parseSchemesStart();
			parseFactsStart();
			parseRulesStart();
			parseQueriesStart();
			if (position != all_tokens.size()){
				error();
			}
			/*
			write << "Success!" << endl; //Print success here!
			write << DL.toString();
			*/
			return true;
		}catch(int i){
			return false;
			/*
		write << "Failure!" << endl;
		write << "  " << all_tokens[position].toString();
		*/
		}//Print Fail inside brackets
	}


//SCHEME FUNCTIONS
	void parseSchemesStart(){
		match(SCHEMES);
		match(COLON);
		parseSchemeList();
	}

	void parseSchemeList(){
		parseScheme();
		if (all_tokens[position].getType() != FACTS)
			parseSchemeList();
	}

	void parseScheme(){
		parsePredicate();
		DL.addScheme(P);    //store predicate here in scheme vector
		P.clear();          //P.delete
	}


//FACT FUNCTIONS
	void parseFactsStart(){
		match(FACTS);
		match(COLON);
		parseFactList();
	}

	void parseFactList(){
		if (all_tokens[position].getType() != RULES){
			parseFact();
			parseFactList();
		}
	}

	void parseFact(){
		parsePredicate();
		DL.addFact(P);
		P.clear();
		match(PERIOD);
	}

//RULE FUNCTIONS
	void parseRulesStart(){
		match(RULES);
		match(COLON);
		parseRuleList();
	}

	void parseRuleList(){
		if (all_tokens[position].getType() != QUERIES){
			parseRule();
			parseRuleList();
		}
	}

	void parseRule(){
		parsePredicate();
		R.setPred(P);
		P.clear();
		match(COLON_DASH);
		parsePredicateList();
		match(PERIOD);
		DL.addRule(R);
		R.clear();
	}


//QUERY FUNCTIONS
	void parseQueriesStart(){
		match(QUERIES);
		match(COLON);
		parseQueryList();
	}

	void parseQueryList(){
		parseQuery();
		if (position < all_tokens.size())
			parseQueryList();
	}

	void parseQuery(){
		parsePredicate();
		DL.addQuery(P);
		P.clear();
		match(Q_MARK);
	}


//PREDICATE FUNCTIONS
	void parsePredicate(){
		match(ID);
		P.setID(all_tokens[position-1].getValue());  //set id for Predicate
		match(LEFT_PAREN);
		parseParameterList();
		//P.addParam(Pa);
		match(RIGHT_PAREN);
	}

	void parsePredicateList(){
		parsePredicate();
		R.addPredList(P);    //put Predicate in Rule
		P.clear();
		if (all_tokens[position].getType() == COMMA){
			match(COMMA);
			parsePredicateList();
		}

	}

//PARAMETER FUNCTIONS
	void parseParameterList(){
		parseParameter();
		P.addParam(Pa);
		//make a Pa
		//put Pa in P
		// clear pa
		if (all_tokens[position].getType() == COMMA){
			match(COMMA);
			parseParameterList();
		}
	}

	void parseParameter(){
		if (all_tokens[position].getType() == STRING){
			match(STRING);
			Pa.setValue(all_tokens[position-1].getValue());
			Pa.isString(true);
			DL.addDomain(all_tokens[position-1].getValue());
		}
		else if (all_tokens[position].getType() == ID){
			match(ID);
			Pa.setValue(all_tokens[position-1].getValue());
			Pa.isString(false);
		}
		else 
			error();
	}


//MATCH TERMINAL 
	void match(TokenType current){
		if (all_tokens[position].getType() == current)
			position++;
		else 
			error();
	}

//ERROR
	void error(){
		throw 1;
	}

	//string of DataLog stuff

};
