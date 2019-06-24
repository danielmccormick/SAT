#include "sat.hh"

namespace sat {
	
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
								std::set<int> clause = parseClause(l);
								
						}
					}					
				}
			} catch (int e) {
				handleError(e);
			}						

		}
		
	}

	std::set<int> formula::parseClause(const std::string &s) {
		if (s.back() != '0') throw 2; 
	}

	void formula::handleError(int i) {
		switch(i) {
			case 0: std::cout << "An error has occured! Invalid File\n";
				break;
			case 1: std::cout << "An error has occured! Invalid Header Line\n";
				break;
			case 2: std::cout << "An error has occured! Invalid Clause Line\n";
			default: 
				std::cout << "An error has occured! Error Code: " << i << "\n";
				break;
			
		}
		return;
	}

} ;
