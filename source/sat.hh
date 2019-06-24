#ifndef SAT_H
#define SAT_H

#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <iostream>

namespace sat {
	inline bool isNum(const char &c) { return (c >= '0' && c <= '9'); }
	
	template<typename T> 
	inline T abs(const T &x) { return ((x > 0) ? x : -x); }

	// formula in NNF or CNF
	class formula {
		public:
			std::vector<std::set<int>> getFormula() { return formula; }
			void setFormula(const std::vector<std::set<int>> &formula_) { formula = formula_; }

			bool getDNF() { return DNF; }
			void setDNF(const bool &DNF_) { DNF = DNF_; } 

			bool readFromFile(const char * &file_path);

			bool validAssignment(const std::map<int,bool> &assignments);

			void DPLL();			

		private:
			bool DNF;
			bool autoSat;
			
			// Returns false if the assignment is neccesarily false
			bool validClause(const std::map<int,bool> &assignments, int clauseNum);

			// Returns false if the assignment is neccesarily false; true otherwise
			bool validVar(const std::map<int,bool> &assignments, int var);		
			
			std::set<int> parseClause(const std::string &s);


		
			void handleError(int); // Error Handler, Reserved for future use
	
	
			std::vector<std::set<int>> formula;	
			
			std::set<int> pureLiterals; // For PLP, reserved for future use
			std::set<int> variables;
	};


};

#endif
