#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <iostream>
#define DEBUG 0

namespace sat {
	inline bool isNum(const char &c) { return (c >= '0' && c <= '9'); }
	
	template<typename T> 
	inline T abs(const T &x) { return ((x > 0) ? x : -x); }
	
	class clause {
		public: 
			clause() = delete;
			clause(const std::vector<int> &); 
			clause(const clause &) = default;

			// valid assignment for cnf only - to see if it's DNF sat use isDNFSat()
			bool validAssignment(const std::map<int,bool> &);
			bool completeAssignment(const std::map<int,bool> &);
			
			// Only meaningful in short circuit
			bool isCNFSat() { return autoValid; }

			bool isDNFSat();
			
			// For unit clause propogation
			bool isUnitClause() { return ( getSize() == 1); }

			// only call getVariable() if not sat, since otherwise it's meaningless
			int getVar() { if (variables.size()) return *variables.begin(); return 0;}
	
			// Don't bother simplifying DNF formulas
			void simplifyClause(const std::map<int,bool> &);

			int getSize() { return variables.size(); }
								
			std::set<int> variables; // If anyone really wants to touch the variables, they can

			void tryClear();
			void setAutoValid(const bool b) { autoValid = b; }
		private: 
			bool autoValid;
	};

	// formula in NNF or CNF
	class formula {
		public:
			std::vector<clause> getFormula() { return formula_; }
			void setFormula(const std::vector<clause> &form) { formula_ = form; }

			bool getDNF() { return DNF; }
			void setDNF(const bool &DNF_) { DNF = DNF_; } 

			bool readFromFile(const char *file_path);

			bool validAssignment(const std::map<int,bool> &assignments);

			void clear();	

			void dumpFormula();

			void simplifyExpression(const std::map<int,bool> &);

			// Takes the assignments 
			bool validAssignment(std::map<int,bool> &);			
			bool completeAssignment(std::map<int,bool> &);

			int getVariable();

			// Do pure logic propogation
			friend std::map<int,bool> PLP(formula &);			

			// Propogate Unit Clauses
			friend void propUnitClauses(std::map<int,bool> &, formula &);
			// is DNF Sat - small function
			friend bool isDNFSat(formula &);

			friend void CNFSimplify(formula &);

			// Will 
			friend bool DPLLSat(formula &f);
			friend bool DPLLInner(formula &f, std::map<int,bool> &);			

		private:
			// Returns false if the assignment is neccesarily false
			bool validClause(const std::map<int,bool> &assignments, int clauseNum);

			// Returns false if the assignment is neccesarily false; true otherwise
			bool validVar(const std::map<int,bool> &assignments, int var);		


			// Simplifies variables out of formula
			bool simplifyFormula(const std::map<int,bool> &);
			
			std::vector<int> parseClause(const std::string &s);

			bool DNF;
			bool autoSat;

			std::vector<clause> formula_;	
			
			std::set<int> variables;
	};

	/* Reserved for future development
	 *	bool CDCLSat(formula &);
 	 *	bool CDCLInner(formula, std::map<int,bool>);
	 */

	void handleError(int);

};
