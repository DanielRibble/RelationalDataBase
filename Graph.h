#pragma once
#include "Node.h"
#include <stack>

using namespace std;

class Graph{
private:
	map<string, Node> graph;
	unsigned po_num;
	vector<string> rule_eval_order;
	set<string> back_edges;

public:
	Graph(){
		po_num = 1;
	}
	~Graph(){}

	void reset(){
		rule_eval_order.clear();
		back_edges.clear();
		po_num = 1;

		map<string, Node>::iterator i;
		for(i = graph.begin(); i != graph.end(); i++){
			i->second.marked = false;
			i->second.po_num = 0;
		}
	}

	void makeNode(string ID){
		Node new_node(ID);
		graph[ID] = new_node;
	}

	void addEdge(string ID, string edge){
		graph[ID].edges.insert(edge);
	}

	vector<string> getRuleOrder(){
		return rule_eval_order;
	}
	

	//DFS
	void dfs(string node_id){
		graph[node_id].marked = true;

		set<string>::iterator i;
		for (i = graph[node_id].edges.begin(); i != graph[node_id].edges.end(); i++){
			if (graph[*i].marked == false){
				dfs(*i);
			}
		}

		graph[node_id].po_num = po_num++;
		if(node_id.substr(0,1) == "R"){
			rule_eval_order.push_back(node_id);
		}
	}


	//Find Backward Edges
	bool findBackEdges(){
		bool back_edge = false;
		

		set<string>::iterator j;
		for (unsigned i = 0; i < rule_eval_order.size(); i++){
			bool insert = false;
			stringstream ss;
			ss << rule_eval_order[i] << ": ";
			for (j = graph[rule_eval_order[i]].edges.begin(); j != graph[rule_eval_order[i]].edges.end(); j++){
				if (graph[rule_eval_order[i]].po_num <= graph[*j].po_num){
					back_edge = true;
					insert = true;
					ss << *j << " ";
				}
			}

			if (insert == true){
				back_edges.insert(ss.str());
			}
		}

		return back_edge;
	}


	//PRINT FUNCTIONS
	//print dependency graph
	void printGraph(ofstream& write){
		write << "Dependency Graph" << endl;
		map<string, Node>::iterator i;
		set<string>::iterator j;
		for (i = graph.begin(); i != graph.end(); i++){
			write << "  " << i->first << ": ";
			for (j = i->second.edges.begin(); j != i->second.edges.end(); j++){
				write << *j << " ";
			}
			write << endl;
		}
	}
	//print post order numbers
	void printPONums(ofstream& write){
		map<string, Node>::iterator i;
		for (i = graph.begin(); i != graph.end(); i++){
			if (i->second.marked == true){
				write << "    " << i->first << ": " << i->second.po_num << endl;
			}
		}
	}
	//print rule evaluation order
	void printRuleEvalOrder(ofstream& write){
		for (unsigned i = 0; i < rule_eval_order.size(); i++){
			write << "    " << rule_eval_order[i] << endl;
		}
	}
	//print back edges
	void printBackEdges(ofstream& write){
		set<string>::iterator i;
		for (i = back_edges.begin(); i != back_edges.end(); i++){
			write << "    " << *i << endl;
		}
	}
};