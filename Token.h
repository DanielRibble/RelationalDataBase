#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

enum TokenType {COMMA,PERIOD,Q_MARK,LEFT_PAREN,RIGHT_PAREN,COLON,COLON_DASH,SCHEMES,FACTS,RULES,QUERIES,ID,STRING};

class Token
{
private:
	TokenType type;
	string value;
	unsigned line_num;
	map<TokenType, string> type_string;

public:
	//CONSTRUCTOR
	Token(TokenType type, string value, unsigned line_num){
		this->type = type;
		this->value = value;
		this->line_num = line_num;

		type_string[COMMA] = "COMMA";
		type_string[PERIOD] = "PERIOD";
		type_string[Q_MARK] = "Q_MARK";
		type_string[LEFT_PAREN] = "LEFT_PAREN";
		type_string[RIGHT_PAREN] = "RIGHT_PAREN";
		type_string[COLON] = "COLON";
		type_string[COLON_DASH] = "COLON_DASH";
		type_string[SCHEMES] = "SCHEMES";
		type_string[FACTS] = "FACTS";
		type_string[RULES] = "RULES";
		type_string[QUERIES] = "QUERIES";
		type_string[ID] = "ID";
		type_string[STRING] = "STRING";
		}
	~Token(){}

	//GETTERS
	TokenType getType(){return type;}

	string getValue(){return value;}

	//TO STRING
	string toString(){
		string s;
		stringstream print;
		print << "(" << type_string[type] << ",\"" << value << "\"," << line_num << ")";
		s = print.str();
		return s;
	}
};