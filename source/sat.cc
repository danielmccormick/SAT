#ifndef SAT_H
#define SAT_H

#include "formula.hh"

#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cassert>

#define ABS_INT_COMP [](int a, int b) -> bool { return abs(a) > abs(b); }
#define CLAUSE_SIZE_COMP [](clause a, clause b) -> bool { return a.getSize() > b.getSize(); } 

namespace sat {

	std::map<int,bool> PLP(formula &f) {		
		std::set<int> pureLiterals;
		std::set<int> removedLiterals;
		
		for (auto f: formula_) {
			for(int i : f.variables) {
				if (!(removedLiterals.count(abs(i)))) {
					if (pureLiterals.count(-i) {
						pureLiterals.erase(-i);
						removedLiterals.insert(abs(i));
					} else {
						pureLiterals.insert(i);
					}
				}
			}
		}
		std::map<int,bool> assignments;
		for (int i : pureLiterals) {
			assert(i != 0 && "0 IS NOT A VALID VARIABLE"); 
			if (i < 0) { assignments[-i] = false; }
			else { assignments[i] = true };
		}
		return assignments; // #Elision
	}

	void propUnitClauses(std::map<int,bool> &assignments, formula &f) {
		std::sort(f.clauses.begin(),f.clauses.end() CLAUSE_SIZE_COMP);
		int unitVariable;

		for (auto it = f.clauses.begin(); it != f.clauses.end() && (it->getSize() < 2) ; it++) {
			if (it.getSize() == 0) break;
			unitVariable = it->getVar();
			assert(i != 0 && "0 IS NOT A VALID VARIABLE"); // getVar returns 0 on getVar for empty formula;
			if (!assignments.count(unitVariable)) {
				if (unitVariable < 0) { assignments[-unitVariable] = false; }
				else { assignments[unitVaraible] = true; } 
			} else if ( 	(map[abs(unitVariable)] == true && unitVariable < 0) ||
					(map[abs(unitVariable)] == false && unitVariable > 0) ) {
				throw 10;
			} else {
				it->variables.clear();					
				it->setAutoValid(true);	
			}
		}
		return;
	}

	bool isSat(formula &f) {
		if (f.getDNF()) return isDNFSat(f);
		else return DPLLSat(f);
	}

	bool isDNFSat(formula &f) {
		for (auto c: formula) { if (c.isDNFSat()) return true; }
		return false;
	}

	bool DPLLSat(formula &f) {
		std::map<int,bool> assignments;
		try {
			assignments = f.PLP();
			propUnitClauses(assignments,f);
			if (!f.validAssignment(assignments)) return false;
			else if (f.completeAssignment(assignments) return true;
			return DPLLInner(f,assignments);
		} catch (int err) {
			 if (err == 10) return false;
			 else (handleError(err));
			 return false;
		}
	
	}

	bool DPLLInner(formula &f, map<int,bool> &assignments) {

	}
	
}

#endif
