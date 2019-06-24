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
	
	class clause {
		clause() : shortResolve(false) {};
		clause(std::vector<int>); 
		bool validAssignment(const std::map<int,bool> &, const bool &);
		
		private: 
			std::set<int> 
			bool shortResolve; // A & Not A

	}	

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
			// Returns false if the assignment is neccesarily false
			bool validClause(const std::map<int,bool> &assignments, int clauseNum);

			// Returns false if the assignment is neccesarily false; true otherwise
			bool validVar(const std::map<int,bool> &assignments, int var);		
			
			clause parseClause(const std::string &s);


			// Error Handler	
			void handleError(int); // Error Handler, Reserved for future use
	
		
			bool DNF;
			bool autoSat;
			std::vector<std::set<int>> formula;	
			
			std::set<int> pureLiterals; // For PLP, reserved for future use
			std::set<int> variables;
	};


};

#endif
