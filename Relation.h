#pragma once
#include "Scheme.h"
#include "Tuple.h"
#include <set>

using namespace std;

class Relation{
private:
	set<Tuple> tuples;
	Scheme scheme;
	string name;

public:
	Relation(){}
	~Relation(){}

	void clear(){
		tuples.clear();
		scheme.clear();
	}

	void addTuple(Tuple tuple){
		tuples.insert(tuple);
	}

	set<Tuple> getTuples(){
		return tuples;
	}

	unsigned getNumOfTuples(){
		return tuples.size();
	}

	void setScheme(Scheme scheme){
		this->scheme = scheme;
	}

	Scheme getScheme(){
		return scheme;
	}

	void setName(string name){
		this->name = name;
	}

	int getAttributePosition(string attribute){
		for (unsigned i = 0; i < scheme.size(); i++){
			if (attribute == scheme[i]){
			return i;
			}
		}

		return -1;
	}


	//SELECT
	Relation select(unsigned pos, string value){
		Relation R;
		R.setName(name);
		R.setScheme(scheme);

		set<Tuple>::iterator i;
		for (i = tuples.begin(); i != tuples.end(); i++){
			if (i->at(pos) == value){
				R.addTuple(*i);
			}
		}

		return R;
	}


	//SPECIAL SELECT
	Relation special_select(unsigned i, unsigned j){
		Relation R;
		R.setName(name);
		R.setScheme(scheme);

		set<Tuple>::iterator it;
		for (it = tuples.begin(); it != tuples.end(); it++){
			if (it->at(i) == it->at(j)){
				R.addTuple(*it);
			}
		}

		return R;
	}


	//PROJECT
	Relation project(vector<unsigned> positions){
		Relation R;
		Scheme S;
		R.setName(name);
		for (unsigned i = 0; i < positions.size(); i++){
			S.push_back(scheme[positions[i]]);
		}
		R.setScheme(S);
		
		Tuple T;
		set<Tuple>::iterator i;
		for (i = tuples.begin(); i != tuples.end(); i++){
			for (unsigned j = 0; j < positions.size(); j++){
				T.push_back(i->at(positions[j]));
			}
			R.addTuple(T);
			T.clear();
		}

		return R;
	}


	//RENAME
	Relation rename(vector<string> new_scheme){		//or maybe change to a type Scheme
		Relation R;
		Scheme S;
		R.setName(name);

		set<Tuple>::iterator i;
		for (i = tuples.begin(); i != tuples.end(); i++){	//copy tuples
			R.addTuple(*i);
		}
		for (unsigned j = 0; j < new_scheme.size(); j++){	//make new scheme from vector of new attribute names (new_scheme)
			S.push_back(new_scheme[j]);
		}
		R.setScheme(S);

		return R;
	}


	//JOIN FUNCTION PIECES (a messy join that depends on Project to clean it up)
	Relation join(Relation& R){								//may or may not want to pass by reference
		Scheme S = R.getScheme();
		Scheme newS = combineSchemes(S);
		set<Tuple> Tset = R.getTuples();

		Relation Rreturn;
		Rreturn.setName(name);								//might be an unneccesary line
		Rreturn.setScheme(newS);

		set<Tuple>::iterator i;
		set<Tuple>::iterator j;
		for (i = tuples.begin(); i != tuples.end(); i++){	//tuples in this object
			for (j = Tset.begin(); j != Tset.end(); j++){	//tuples in imported object
				if (canJoin(*i, scheme, *j, S) == true){
					Rreturn.addTuple(joinTuple(*i, *j));
				}
			}
		}

		return Rreturn;
	}

	Scheme combineSchemes(Scheme& S){
		Scheme Sreturn = scheme;
		//bool found = false;
		for (unsigned i = 0; i < S.size(); i++){
			Sreturn.push_back(S[i]);
			/*found = false;
			for (unsigned j = 0; j < Sreturn.size(); j++){
				if (Sreturn[j] == S[i]){
					found = true;
				}
			}
			if (found == false){
				Sreturn.push_back(S[i]);
			}*/
		}
		return Sreturn;
	}

	bool canJoin(const Tuple& t1, Scheme& s1, const Tuple& t2, Scheme& s2){
		for (unsigned i = 0; i < t1.size(); i++){
			for (unsigned j = 0; j < t2.size(); j++){
				if (s1[i] == s2[j] && t1[i] != t2[j]){
					return false;
				}
			}
		}
		return true;
	}

	Tuple joinTuple(const Tuple& t1, const Tuple& t2){				// or (const Tuple& t1, Scheme& s1, const Tuple& t2, Scheme& s2){
		Tuple T = t1;
		//bool found = false;
		for (unsigned i = 0; i < t2.size(); i++){
			T.push_back(t2[i]);
			/*found = false;
			for (unsigned j = 0; j < t1.size(); j++){
				if (s1[j] == s2[i]){
					found = true;
				}
			}
			if (found == false){
				T.push_back(t2[i]);
			}*/
		}
		return T;
	}


};
