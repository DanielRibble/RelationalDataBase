#pragma once
#include "Token.h"
#include <vector>
#include <fstream>
#include <cctype>
#include <set>

using namespace std;

class Scanner
{
private:
	vector<Token> all_tokens;
	TokenType type;
	bool error;
	bool ignore;
	set<string> key_words;
	unsigned line_num;

public:
	Scanner(){
		error = false;
		ignore = false;
		line_num = 1;
		key_words.insert("Schemes");
		key_words.insert("Facts");
		key_words.insert("Rules");
		key_words.insert("Queries");
	}
	~Scanner(){}

	//GET VECTOR OF ALL TOKENS
	vector<Token> getTokens(){return all_tokens;}


	//INITIAL SCAN
	bool scan(ifstream& read){
		char c;
		string wsf; //wsf = word so far

		while (error == false && read.get(c)){
			ignore = false;
			wsf.push_back(c);

			if (isalpha(c)){
				s_id(read, wsf);
			}
			else if (ispunct(c)){
				s_punct(read, wsf);
			}
			else if (isspace(c)){
				wsf.pop_back();
				if (c == '\n')
					line_num++;
			}
			else{
				error = true;
			}
			

			if (error == true){
				return false;
			}

			wsf.clear();
			/*
			Token t(type, wsf, line_num);
			all_tokens.push_back(t);
			*/
		}

		return true;
	}


	//READS TOKENS BEGINNING WITH LETTERS
	void s_id(ifstream& read, string& wsf){
		char c;
		while (read.get(c) && (isalpha(c) || isdigit(c))){
			wsf.push_back(c);
		}
		
		read.unget();

		if (key_words.count(wsf) == 1)
			s_key_word(read, wsf);
		else
			type = ID;

		if (error == false){
			Token t(type, wsf, line_num);
			all_tokens.push_back(t);
		}
	}


	//READS KEY WORDS
	void s_key_word(ifstream& read, string& wsf){
		if (wsf == "Schemes")
			type = SCHEMES;
		else if (wsf == "Facts")
			type = FACTS;
		else if (wsf == "Rules")
			type = RULES;
		else if (wsf == "Queries")
			type = QUERIES;
	}


	//READS PUNCTUATION
	void s_punct(ifstream& read, string& wsf){
		if (wsf == ":"){
			char c;
			read.get(c);
			if (c == '-'){
				wsf.push_back(c);
				type = COLON_DASH;
			}
			else{
				read.unget();
				type = COLON;
			}
		}
		else if (wsf == "#"){
			string ignoring;
			getline(read,ignoring);
			ignore = true;
			line_num++;
		}
		/*else if (wsf == ",")
			type = COMMA;
		else if (wsf == ".")
			type = PERIOD;
		else if (wsf == "?")
			type = Q_MARK;
		else if (wsf == "(")
			type = LEFT_PAREN;
		else if (wsf == ")")
			type = RIGHT_PAREN;
		else if (wsf == "'")
			s_string(read, wsf);*/
		else 
			error = s_punct_cont(read, wsf);
		
		if (error == false && ignore == false){
			Token t(type, wsf, line_num);
			all_tokens.push_back(t);
		}
	}

	bool s_punct_cont(ifstream& read, string& wsf){
		if (wsf == ",")
			type = COMMA;
		else if (wsf == ".")
			type = PERIOD;
		else if (wsf == "?")
			type = Q_MARK;
		else if (wsf == "(")
			type = LEFT_PAREN;
		else if (wsf == ")")
			type = RIGHT_PAREN;
		else if (wsf == "'")
			return s_string(read, wsf);
		else 
			return true;

		return false;
	}

	//READS STRING
	bool s_string(ifstream& read, string& wsf){
		char c;
		wsf.pop_back();
		while (read.get(c) && c !='\''){ 
			if (c == '\n'){
				error = true;
				return true;
			}
			wsf.push_back(c);
		}

		if (c != '\''){
			error = true;
			return true;
		}

		type = STRING;

		return false;

	}

	//PRINTS ERROR MESSAGE
	void print_error(ofstream& write){
		for (unsigned i = 0; i < all_tokens.size();i++){
			write << all_tokens[i].toString() << endl;
		}

		write << "Input Error on line "<< line_num;
	}

	//PRINTS ALL TOKENS
	void print_tokens(ofstream& write){
		for (unsigned i = 0; i < all_tokens.size();i++){
			write << all_tokens[i].toString() << endl;
		}
		write << "Total Tokens = " << all_tokens.size();
	}

};
