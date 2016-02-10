#pragma once
#include "Parser.h"
#include "DataBase.h"
#include "Graph.h"

using namespace std;

class Interpreter{
private:
	DataBase DB;
	Relation R;
	Tuple T;
	Scheme S;
	Graph G;
	map<string, Rule> rule_map;

public:
	Interpreter(){}
	~Interpreter(){}

	void interpret(DataLog DL, ofstream& write){
		vector<Predicate> schemes = DL.getSchemes();
		vector<Predicate> facts = DL.getFacts();
		vector<Predicate> queries = DL.getQueries();
		vector<Rule> rules = DL.getRules();

		createRelations(schemes, facts);							//fill DataBase with relations from schemes and facts

		createGraph(rules, queries);								//make Dependency Graph from rules and queries
		G.printGraph(write);

		//processAllRules(rules, write);

		processAllQueries(queries, write);

	}


	//Processes Schemes and Facts, while creating Relations.
	void createRelations(vector<Predicate>& schemes, vector<Predicate>& facts){
		string name;
		vector<Parameter> parameters;

		for (unsigned i = 0; i < schemes.size(); i++){				//steps through schemes
			name = schemes[i].getID();
			parameters = schemes[i].getParameterList();
			for (unsigned j = 0; j < parameters.size(); j++){		//steps through scheme parameters
				S.push_back(parameters[j].toString());
			}

			R.setName(name);
			R.setScheme(S);
			S.clear();

			for (unsigned j = 0; j < facts.size(); j++){			//steps through facts to match schemes to facts
				if (facts[j].getID() == name){
					parameters = facts[j].getParameterList();
					for (unsigned k = 0; k < parameters.size(); k++)	//steps through fact parameters to add to list of tuples
					{
						T.push_back(parameters[k].toString());
					}
					R.addTuple(T);
					T.clear();
				}
			}

			DB.addRelation(name,R);
			R.clear();
		}
	}


	//GRAPH PROCESSING
	//Create graph from Rules and Queries
	void createGraph(vector<Rule>& rules, vector<Predicate>& queries){
		for(unsigned i = 0; i < queries.size(); i++){						//graph queries and dependencies
			stringstream ss;
			ss << "Q" << i+1;
			string q_node = ss.str();
			G.makeNode(q_node);

			for(unsigned j = 0; j < rules.size(); j++){
				if(queries[i].getID() == rules[j].getPredicate().getID()){
					ss.str("");
					ss << "R" << j+1;
					G.addEdge(q_node, ss.str());
				}
			}
		}

		for(unsigned i = 0; i < rules.size(); i++){						//graph rules and dependencies
			stringstream ss;
			ss << "R" << i+1;
			string r_node = ss.str();
			G.makeNode(r_node);

			rule_map[r_node] = rules[i];

			vector<Predicate> rule_body = rules[i].getPredicateList();
			for(unsigned j = 0; j < rule_body.size(); j++){
				for(unsigned k = 0; k < rules.size(); k++){
					if(rules[k].getPredicate().getID() == rule_body[j].getID()){
						ss.str("");
						ss << "R" << k+1;
						G.addEdge(r_node, ss.str());
					}
				}
			}
		}
	}
		


	//RULES PROCESSING
	//Process Rules one by one and PRINTS the number of passes
	void processAllRules(vector<Rule>& rules, bool& back_edges, ofstream& write){
		unsigned passes = 0;
		unsigned originalTupleNumber = DB.getTotalTupleCount();
		bool changes = true;
		do{
			for (unsigned i = 0; i < rules.size(); i++){
				write << "    " << rules[i].toString() << endl;
				processRule(rules[i]);
			}
			passes++;
			if (originalTupleNumber == DB.getTotalTupleCount()){
				changes = false;
			}
			originalTupleNumber = DB.getTotalTupleCount();
		}while (changes == true && back_edges == true);

		//write << "Converged after " << passes << " passes through the Rules." << endl;
		
	}


	//Process one individual Rule
	void processRule(Rule& rule){
		vector<Predicate> rulePredicateList = rule.getPredicateList();
		R.clear();
		processQuery(rulePredicateList[0]);
		Relation newR = R;												//newR is relation to be modified. R is modified by processQuery
		//R = DB.getRelation(ruleRelations[0].getID());
		for (unsigned i = 1; i < rulePredicateList.size(); i++){			//Joining right side of Rule
			processQuery(rulePredicateList[i]);							//changes R each time
			newR = newR.join(R);
			//R = R.join(DB.getRelation(ruleRelations[i].getID()));
		}

		Predicate leftRule = rule.getPredicate();
		vector<unsigned> ID_positions;
		vector<Parameter> parameter_list = leftRule.getParameterList();
		for (unsigned i = 0; i < parameter_list.size(); i++){								//Convert parameters of left side of rule into positions to project
			ID_positions.push_back(newR.getAttributePosition(parameter_list[i].toString()));
		}

		newR = newR.project(ID_positions);

		DB.relationUnion(leftRule.getID(), newR.getTuples());				//Union with stored relation in DB

	}


	//QUERIES PROCESSING
	//Process Queries one by one and PRINT out after each
	void processAllQueries(vector<Predicate>& queries, ofstream& write){
		for (unsigned i = 0; i < queries.size(); i++){
			write << endl << queries[i].toString() << "?" << endl << endl;
			
			write << "  Postorder Numbers" << endl;
			stringstream ss;												//post order numbers
			ss << "Q" << i+1;
			G.dfs(ss.str());
			G.printPONums(write);

			write << endl;													//rule evaluation order
			write << "  Rule Evaluation Order" << endl;
			G.printRuleEvalOrder(write);

			bool back_edges = G.findBackEdges();							//Backward edges
			write << endl;
			write << "  Backward Edges" << endl;
			G.printBackEdges(write);

			write << endl << "  Rule Evaluation" << endl;					//Rule evaluation
			vector<string> rule_eval_order = G.getRuleOrder();
			vector<Rule> rules_to_eval;
			for(unsigned j = 0; j < rule_eval_order.size(); j++){
				rules_to_eval.push_back(rule_map[rule_eval_order[j]]);
			}
			processAllRules(rules_to_eval, back_edges, write);
			write << endl;
			
			processQuery(queries[i]);										//process and print query result
			print(queries[i], write);

			R.clear();
			G.reset();
		}
	}

	//Process one query
	void processQuery(Predicate& query){
		R = DB.getRelation(query.getID());
		vector<unsigned> ID_positions;
		vector<string> IDs_found;
		map<string, unsigned> ID_combo;									//combination of IDs found and position they are found in.
		vector<Parameter> q_parameters = query.getParameterList();
		
		for (unsigned i = 0; i < q_parameters.size(); i++){				//call select on each string
			if (q_parameters[i].isItAString() == true){
				R = R.select(i, q_parameters[i].toString());
			}
			else{
				if (ID_combo.count(q_parameters[i].toString()) > 0){
					unsigned first_pos = ID_combo[q_parameters[i].toString()];
					R = R.special_select(first_pos, i);
				}
				else{
					ID_positions.push_back(i);
					IDs_found.push_back(q_parameters[i].toString());
					ID_combo[q_parameters[i].toString()] = i;
				}
			}
		}
		R = R.project(ID_positions);
		R = R.rename(IDs_found);
	}


	//PRINT
	void print(Predicate& query, ofstream& write){
		S = R.getScheme();
		set<Tuple> tuples = R.getTuples();
		write << query.toString() << "? ";
		if (tuples.size() < 1){										//If tuples set is empty, print NO
			write << "No" << endl;
		}
		else{
			write << "Yes(" << tuples.size() << ")" << endl;		//If tuples set is not empty, print yes and output size of set
			if (S.size() > 0){										//If Scheme vector is above zero, print out possible combination of variables
				set<Tuple>::iterator i;
				for (i = tuples.begin(); i != tuples.end(); i++){	//Step through set
					write << "  ";
					for (unsigned j = 0; j < S.size(); j++){		//Step through tuple in set
						write <<S[j] << "=" << i->at(j);
						if (j != (S.size()-1)){
							write << ", ";
						}
					}
					write << endl;
				}
			}
		}
		//S.clear();
	}

};
