#include <stdio> 
#include <string>
#include <set>

namespace proplogic
{
enum logicType { UNARY, BINARY, VARIABLE, LITERAL }
enum connectiveType {AND, OR, NOT, IMPLIES, IFF, XOR, NIL } // Nil is default - (). (P) = P. ALso literal in this context is a true or false value.

class node {
	class friend formula;
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

node::node() {
	type = VARIABLE;
	connective = NIL:
	truth = false;
	truthSet = false;
	name = "";
	lChild = NULL;
	rChild = NULL;
}

node::node(node *lChild_, node *rChild_, const std::string &name_, logicType type_, connectiveType connective_) {
	type = type_;
	connective = connective_;
	name = name_;
	truth = false;
	truthSet = false;
	lChild = lChild_;
	rChild = rChild_;
}

node::node(bool truth_) {
	type = LITERAL;
	connective = NIL;
	truth = val;
	truthSet = true;
	lChild = NULL;
	rChild = NULL:
}
	
void node::printVar() {
	if (type == LITERAL) {
		if (!truthSet) {
			std::cout << "ERROR: PRINTING INVALID LITERAL\n";
			return;
		} else if (truth) {
			std::cout << "T ";
			return;
		} else {
			std::cout << "F "; 
		}
	} else if (type == VARIABLE) {
		if (name.empty()) {
			if (truthSet) {
				if (truth) std::cout << "T ";
				else std::cout << "F ";
				return;
			}
			else {
				std::cout << "WARNING: NO NAME SET IN PRINT STATEMENT\n";
				return;
			}
		}
	}
	else {
		std::cout << "ERROR: CALLED TO PRINT VARIABLE ON A CONNECTIVE\n";
	}
	return;
}

void node::printConnective() {
	switch(type) {
		case LITERAL :	std::cout << "(literal) ";
				break;
		case VARIABLE : std::cout << "(variable) ";
				break;
		case OR : 	std::cout << "OR ";
				break;
		case AND : 	std::cout << "AND ";
				break;
		case IMPLIES : 	std::cout << "--> ";
				break;
		case IFF : 	std::cout << "<--> ";
				break;
		case XOR : 	std::cout << "XOR ";
				break;
		case NIL : 	break;
	}
	return;
}



class proposition {
	public:
		node *head;
			
		void printTraversal(); //  This is an incomplete
		
		node* getHead() { return head; } 
		void setHead(const node * head_) { head = head_; }
	
		void NNF(); // will convert to NNF in the future
		bool eval(std::vector<bool>);
	private:
		void printInner(node *)
	
		void serializeVariables(); // will serialize inorder. This will get all variables into the literal list
		void serialInner(node *);
		void assign(std::vector<bool>)
		
		std::set<node *> variableList; 
}


void proposition::printTraversal() {
	printInner(head);
	return;
}

void proposition::printInner(node *curr) {
	if (curr == NULL) return;
	else if (curr->type == VARIABLE || type == LITERAL) curr->printVar();
	else if (type == UNARY) {
		curr->printConnective(); // Prints the connective name
		printInner(curr->getLChild()); // should be "Not"
	} else if (type == BINARY) {
		printInner(curr->getLChild());
		curr->printConnective();
		printInner(curr->getRChild());
	} else {
		std::cout << "Unknown logic type!!! Failure" << std::endl;
		std::terminate();
		return;
	}
 	return 0;
}

void proposition::serializeVariables() {
	if (head == NULL) return;
	variableList.clear();
	serializeInner(head);	
}

void proposition::serializeInner(node *curr) {
	if (node == NULL) return;
	serializeInner(curr->getLChild());
	if (curr->getType() == VARIABLE && !variableList.count(curr)) {
		variableList.insert(curr);	
	}	
	serializeInner(curr->getRChild());
	return;
} 
