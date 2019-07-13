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
	
	void CNFSimplify(sat::formula &f) {
		for (auto c : f.formula_) {
			std::set<int> vars;
			for (auto i : c.variables) {
				if (vars.count(-i)) {
					c.variables.clear();
					vars.clear();
					c.setAutoValid(true);
					break;
				} else { 
					vars.insert(i);
				}
			}
			vars.clear();
		}
		return;
	}

	std::map<int,bool> PLP(formula &form) {		
		std::set<int> pureLiterals;
		std::set<int> removedLiterals;
		
		for (auto f: form.formula_) {
			for(int i : f.variables) {
				if (!(removedLiterals.count(abs(i)))) {
					if (pureLiterals.count(-i)) {
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
			else { assignments[i] = true; }
		}
		return assignments; // #Elision
	}

	void propUnitClauses(std::map<int,bool> &assignments, formula &f) {
		std::sort(f.formula_.begin(),f.formula_.end(), CLAUSE_SIZE_COMP);
		int unitVariable;

		for (auto it = f.formula_.begin(); it != f.formula_.end() && (it->getSize() < 2) ; it++) {
			if (it->getSize() == 0) break;
			unitVariable = it->getVar();
			assert(unitVariable != 0 && "0 IS NOT A VALID VARIABLE"); // getVar returns 0 on getVar for empty formula;
			if (!assignments.count(unitVariable)) {
				if (unitVariable < 0) { assignments[-unitVariable] = false; }
				else { assignments[unitVariable] = true; } 
			} else if ((assignments[abs(unitVariable)] == true && unitVariable < 0) || (assignments[abs(unitVariable)] == false && unitVariable > 0) ) { 
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
		for (auto c: f.formula_) { if (c.isDNFSat()) return true; }
		return false;
	}

	bool DPLLSat(formula &f) {
		std::map<int,bool> assignments;
		try {
			CNFSimplify(f);
			assignments = PLP(f);
			propUnitClauses(assignments,f);
			if (!f.validAssignment(assignments)) return false;
			else if (f.completeAssignment(assignments)) return true;
			return DPLLInner(f,assignments);
		} catch (int err) { // If an error is thrown in DPLLInner it'll be caught here .. eh
			 if (err == 10) return false;
			 else (handleError(err));
			 return false;
		}	
	}

	/*
 	 * So we aren't doing pure literal propogation, but it could be useful
	 * An O(n) algorithm being done 2^n times sounds very expensive though
	 * so I'm unsure if it'd have a performance increase. In meantime,
 	 * since it's not a requirement of DPLL, we only do it off the bat, just
 	 * in case we can clean up the formula quickly.
     	 */
	bool DPLLInner(formula &f, std::map<int,bool> &assignments) {
		propUnitClauses(assignments,f); 			// Propogate all unit clauses
		f.simplifyExpression(assignments); 			// Simplify this formula
		if (!f.validAssignment(assignments)) return false; 	// If there's a conflict, this branch ends here
		if (f.completeAssignment(assignments)) return true; 	// If there's a satisfying assignment (we don't 
									// need all variables) return true.
		formula newForm(f);
		std::map<int,bool> updatedAssignments(assignments);		// Copy CTORs probably eat a lot of resources
		int var = newForm.getVariable();
		updatedAssignments[var] = true;				// This the decision section
		if (DPLLInner(newForm,updatedAssignments)) return true;
		updatedAssignments[var] = false;	
		if (DPLLInner(newForm,updatedAssignments)) return true;
		return false; // If setting variable either way returns false, return false.
	}	
};
