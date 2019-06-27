#include "sat.hh"
#include <memory>
#define ABS_INT_COMP [](int a, int b) -> bool { return abs(a) > abs(b); }

namespace sat {
	
	clause::clause(const std::vector<int> &vec) {
		shortResolve = false;
		autoValid = false;
		for (auto i : vec) {
			if (variables.count(-i)) { shortResolve = true; }
			variables.insert(i);
		}
		return;				
	}

	bool clause::validAssignment(const std::map<int,bool> &assignments) {
		if ( isCNFSat() ) { return true; }
		if ( variables.empty() ) { return false; } 
		for (auto i : variables) { /// Needs only one example
			if (assignments.count(abs(i))) {
				if ( (assignments.find(abs(i)))->second == true && i > 0) return true;
				if ( (assignments.find(abs(i)))->second == false && i < 0) return true;	
			}
		}
		return false;
	}

	void clause::simplifyFormula(const std::map<int,bool> &assignments) {
		for (auto i : variables) {
			if (assignments.count(abs(i))) {
				if ( (assignments.find(abs(i)))->second == true && i > 0) {
					autoValid = true;
				} else if ( (assignments.find(abs(i)))->second == false && i < 0) {
					autoValid = true;
				}
				variables.erase(i);
			}
		}
		return; 
	}
	
	bool formula::readFromFile(const char *&file_path) {
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
				std::unique_ptr<std::set<int>> notPureLiterals = std::make_unique<std::set<int>>();
				while (getline(file,l)) {
					if (l.empty()) { throw 0; }
					else if (l[0] == 'c') { }
					else if (l[0] == 'p') {
						if (l.size() < 5) { throw 1; }
						else if (l.substr(3,3) == "dnf") { DNF = true; }
						else if (l.substr(3,3) == "cnf") { DNF = false; } 
						else { throw 1; }
						headerFound = true;
					} else if (isNum(l[0]) || l[0] == '-') {
						if (headerFound == false) { return false; }
						{
							std::vector<int> parsedClause = parseClause(l);
							clauseFound = true;
							for (auto i : parsedClause) {
								if (!notPureLiterals->count(abs(i))) {
									if (pureLiterals.count(-i)) {
										pureLiterals.erase(-i);
										notPureLiterals->insert(abs(i));
									} else { pureLiterals.insert(i); }
								}
								variables.insert(abs(i));
							}
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
		int idx, offset;
		while ( static_cast<unsigned int>(idx) < s.size()) {
			while (! ( isNum(s[idx]) || s[idx] == '-')) { // parse whitespace
				if (s[idx] != ' ') { throw 3; }
				idx++;
			}
			offset = 0;
			if (s[idx] == '-') {
				if (!isNum(s[idx])) { throw 4; }
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

	std::map<int,bool> formula::PLP() {
		std::map<int,bool> assignments;
		for (auto i : pureLiterals) {
			if (i < 0) assignments[-i] = false;
			else assignments[i] = true;
		}
		return assignments;
	}

	void formula::handleError(int i) {
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

			default: 
				std::cout << "An error has occured! Error Code: " << i << "\n";
				break;
			
		}
		return;
	}

	void formula::clear() {
		formula_.clear();
		pureLiterals.clear();
		variables.clear();
	}

} ;
