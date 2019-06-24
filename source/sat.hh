#ifndef SAT_H
#define SAT_H

#include <set>
#include <vector>
#include <map>

namespace sat {
	
	// formula in NNF or CNF
	class formula {
		public:
			std::vector<std::set<int>> getFormula() { return formula; }
			void setFormula(const std::vector<std::set<int>> &formula_) { formula = formula_; }

			bool getDNF() { return DNF };
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
			void handleError(int);
	
	
			std::vector<std::set<int>> formula;	
			std::set<int> variables;
	};








};
