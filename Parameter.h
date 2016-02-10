#pragma once
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Parameter
{
private:
	string info;
	bool is_string;

public:
	Parameter(){}
	~Parameter(){}

	void setValue(string value){
		info = value;
	}

	void isString(bool t_f){
		is_string = t_f;
	}

	bool isItAString(){
		return is_string;
	}



	string toString(){
		string s;
		stringstream ss;
		if(is_string){
			ss << "'" << info << "'";
		}
		else {
			ss << info;
		}

		s = ss.str();
		return s;
	}
};