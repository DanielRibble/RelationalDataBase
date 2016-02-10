#pragma once
#include <string>
#include <set>

using namespace std;

struct Node{
	string name;
	set<string> edges;
	bool marked;
	unsigned po_num; //post order number

public:
	Node(){
		name = "";
		marked = false;
		po_num = 0;
	}
	Node(string name){
		this->name = name;
		marked = false;
		po_num = 0;
	}
};