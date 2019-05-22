#include <iostream> 
#include <string>
#include <set>

namespace proplogic
{
enum logicType { UNARY, BINARY, VARIABLE, LITERAL };
enum connectiveType {AND, OR, NOT, IMPLIES, IFF, XOR, NIL }; // Nil is default - (). (P) = P. ALso literal in this context is a true or false value.

class node {
	friend class formula;

	public:
		node();
		node(node *, node *, const std::string &, logicType, connectiveType);
		node(bool);
	
		node* getlChild() { return lChild; }
		node* getrChild() { return rChild; }

		void setlChild(node *lChild_) { lChild = lChild_; }
		void setrChild(node *rChild_) { rChild = rChild_; } 

		std::string getName() { return name; }
		void setName(const std::string &name_) { name = name_; }

		bool getTruth() {if (!truthSet) { std::cout << "WARNING: CALLING getTruth() ON UNASSIGNED LOGIC"; } return truth; }
		bool setTruth(const bool truth_) { truthSet = true; truth = truth_;}
		bool unsetTruth() {truthSet = false;}
	
		logicType getLogicType() { return type; } 
		void setLogicType(const logicType _type) { type = _type; };

		connectiveType getConnectiveType() { return connective; }
		void setConnectiveType(const connectiveType connective_) { connective = connective_; }

		void printVar(); // Will print the variable for us
		void printConnective(); // will print the connective for us

	private:
		logicType type; // Only leafs should be propositional variables
		connectiveType connective; // Default connective is NOR; an a propositional variable can have a "NOT" connective in this implementation.
 
		bool truth; // optional statement unless a literal
		bool truthSet; // truthSet should be set to false; 
		std::string name;
		node *lChild; // In the case of a unary operator, the child will be pointed to from lChild
		node *rChild;
};

class proposition {
	public:
		node *head;
			
		void printTraversal(); //  This is an incomplete
		
		node* getHead() { return head; } 
		void setHead(node* const head_) { head = head_; }
	
		void NNF(); // will convert to NNF in the future
		bool eval(std::vector<bool>);
	private:
		void printInner(node *);
	
		void serializeVariables(); // will serialize inorder. This will get all variables into the literal list
		void serializeInner(node *);
		void assign(const std::vector<bool> &);
		
		std::set<node *> variableList; 
};

} 
