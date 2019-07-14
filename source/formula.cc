#include "formula.hh"
#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cassert>
#define DEBUG 0

#define ABS_INT_COMP [](int a, int b) -> bool { return abs(a) < abs(b); }

void dumpMap(const std::map<int,bool> &m) {
	if (!DEBUG) return;
	std::cout << "Map Dump:\n"; 
	for (auto it = m.begin(); it != m.end(); it++) {
		std::cout << it->first << " -> " << it->second << "\n";
	}
}

void dumpSet(std::set<int> &s) {
	if (!DEBUG) return;
	std::cout << "Set Dump: ";
	for (auto i : s) {
		std::cout << i << ",";
	}
	std::cout << "\n";
}

namespace sat {
	
	clause::clause(const std::vector<int> &vec) {
		autoValid = false;
		for (auto i : vec) {
			if (variables.count(-i)) { setAutoValid(true); }
			variables.insert(i);
		}
		return;				
	}

	bool clause::validAssignment(const std::map<int,bool> &assignments) {
		if ( isCNFSat() ) { return true; }
		if ( variables.empty() ) { return false; } 
		for (auto i : variables) { /// Needs only one example
			assert(i != 0 && "0 IS NOT A VALID VARIABLE"); 
			if (assignments.count(abs(i))) {
				if ( (assignments.find(abs(i)))->second == true && i > 0) return true;
				if ( (assignments.find(abs(i)))->second == false && i < 0) return true;	
			} else {
				return true; 
			}
		}
		return false; // No unassigned or correctly assigned variables
	}

	bool clause::isDNFSat() {
		std::set<int> vars;
		for (auto i : variables) {
			if (vars.count(-i)) return false;
			vars.insert(i);
		}
		return true;
	}

	/*
	 * The key difference between valid and complete
	 * is that an unassigned variable does not satisfy
	 * complete. This is because complete implies we're done
  	 */ 

	bool clause::completeAssignment(const std::map<int,bool> &assignments) {
		if ( isCNFSat() ) { return true; }
		if ( variables.empty() ) { return false; } 
		for (auto i : variables) { /// Needs only one example
			assert(i != 0 && "0 IS NOT A VALID VARIABLE"); 
			if (assignments.count(abs(i))) {
				if ( (assignments.find(abs(i)))->second == true && i > 0) return true;
				if ( (assignments.find(abs(i)))->second == false && i < 0) return true;	
			}
		}
		return false;

	}

	void clause::simplifyClause(const std::map<int,bool> &assignments) {
		dumpMap(assignments);
		dumpSet(variables);
		for (auto it = variables.begin(); it != variables.end(); it++) {
			if (assignments.count(abs(*it))) {
				assert(*it != 0 && "0 IS NOT A VALID VARIABLE"); 
				if ( (assignments.find(abs(*it)))->second == true && *it > 0) {
					//std::cout << "Branch 1 Taken: Initial size : " << variables.size() << ", final size: ";			
					autoValid = true;
					variables.clear();
					//std::cout << variables.size() << "\n";
					dumpSet(variables);
					return;
				} else if ( (assignments.find(abs(*it)))->second == false && *it < 0) {
					//std::cout << "Branch 2 Taken: Initial size : " << variables.size() << ", final size: ";	
					autoValid = true;
					variables.clear();
					//std::cout << variables.size() << "\n";
					dumpSet(variables);
					return;
				} else {
					//std::cout << "Branch 3 Taken: Initial size : " << variables.size() << ", final size: ";
					variables.erase(*it);
					it = variables.begin();
					it--;
					//std::cout << variables.size() << "\n";
				}
			}
		}
		dumpSet(variables);
		return; 
	}
	
	bool formula::readFromFile(const char *file_path) {
		if (file_path == NULL) return false;
		std::ifstream file(file_path);
		if (!file.is_open()) {
			handleError(0); // hacky but this should be ok
			return false;			
		} else { 
			bool headerFound = false;
			bool clauseFound = false;	
			std::string l;
			try {
				while (getline(file,l)) {
					if (l.empty()) { throw 0; }
					else if (l[0] == 'c') { }
					else if (l[0] == 'p') {
						if (l.size() < 5) {
							throw 1;
						}
						else if (l.substr(2,3) == "dnf") { DNF = true; }
						else if (l.substr(2,3) == "cnf") { DNF = false; } 
						else {
							throw 1; 
						}
						headerFound = true;
					} else if (isNum(l[0]) || l[0] == '-') {
						if (headerFound == false) { return false; }
						{
							std::vector<int> parsedClause = parseClause(l);
							clauseFound = true;
							for (auto i : parsedClause) { variables.insert(abs(i)); }
							formula_.push_back(clause(parsedClause));
						}
					}					
				}
			} catch (int e) {
				handleError(e);
			}						
			if (headerFound && clauseFound) return true;
			return false;
		}
		
	}

	std::vector<int> formula::parseClause(const std::string &s) {
		if (s.back() != '0') throw 2; 
		std::vector<int> clause;
		int idx = 0, offset;
		while ( static_cast<unsigned int>(idx) < s.size()) {
			while (! ( isNum(s[idx]) || s[idx] == '-')) { // parse whitespace
				if (s[idx] != ' ') { throw 3; }
				idx++;
			}
			offset = 0;
			if (s[idx] == '-') {
				if (!isNum(s[idx+1])) {
					throw 4;
				 }
				offset = offset+1;
			}
			while ( static_cast<unsigned int>(idx+offset) < s.size() && isNum(s[idx+offset])) { // iterate until end of number
				offset += 1;
			}
			if ( static_cast<unsigned int>(idx+offset) < s.size() && s[idx+offset] != ' ')  { throw 3; }
			if (std::stoi(s.substr(idx,offset)) == 0) {
				std::sort(clause.begin(),clause.end(), ABS_INT_COMP); 
				return clause;
			}
			clause.push_back(std::stoi(s.substr(idx,offset)));
			idx = (idx+offset)+1;	
		}	
		throw 2;
	}

	void formula::simplifyExpression(const std::map<int,bool> &variables) {
		for (auto c : formula_) { c.simplifyClause(variables); }
		return;
	}

	void formula::clear() {
		formula_.clear();
		variables.clear();
	}

	bool formula::validAssignment(std::map<int,bool> &variables) {
		for (auto c: formula_) {
			if (!(c.validAssignment(variables))) return false;
		}
		return true;
	}

	bool formula::completeAssignment(std::map<int,bool> &variables) {
		for (auto c: formula_) {
			if (!(c.completeAssignment(variables))) return false;
		}
		return true;
	}

	int formula::getVariable() {
		for (auto c : formula_) {
			if (c.getSize() > 0) {
				dumpSet(c.variables);
				return c.getVar();
			}
		}
		throw 10;
	}

	void formula::dumpFormula() {
		if (!DEBUG) return;
		std::cout << "Dumping Formula:\n";
		for (auto c : formula_) {
			for (auto it : c.variables) {
				std::cout << it << ", ";
			}
			std::cout << "\n";
		}
	}

	void handleError(int i) {
		switch(i) {
			case 0: std::cout << "An error has occured! Invalid File\n";
				break;
			case 1: std::cout << "An error has occured! Invalid Header Line\n";
				break;
			case 2: std::cout << "An error has occured! Clause Line Unterminated\n";
				break;
			case 3: std::cout << "An error has occured! Clause Line has Invalid Whitespace\n";
				break;
			case 4: std::cout << "An error has occured! Clause Line Invalid (Double \'-\')\n";
				break;
			case 5: std::cout << "An error has occured! Reached end of evaluating clause with no decision\n";
				break;
			case 6: break; // Reserve
			default: 
				std::cout << "An error has occured! Error Code: " << i << "\n";
				break;
			
		}
		return;
	}

} ;
