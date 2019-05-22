#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include "proplogic.h"

namespace proplogic {

node::node() {
        type = VARIABLE;
        connective = NIL:
        truth = false;
        truthSet = false;
        name = "";
        lChild = NULL;
        rChild = NULL;
}

node::node(node *lChild_, node *rChild_, const std::string &name_, logicType type_,
connectiveType connective_) {
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
                case LITERAL :  std::cout << "(literal) ";
                                break;
                case VARIABLE : std::cout << "(variable) ";
                                break;
                case OR :       std::cout << "OR ";
                                break;
                case AND :      std::cout << "AND ";
                                break;
                case IMPLIES :  std::cout << "--> ";
                                break;
                case IFF :      std::cout << "<--> ";
                                break;
                case XOR :      std::cout << "XOR ";
                                break;
                case NIL :      break;
		default : 	std::cout << "ERROR: LOGICTYPE SOMEHOW IS NOT STANDARD CONNECTIVE" << std::endl;
        }
        return;
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
        return;
}

void proposition::serializeVariables() {
        if (head == NULL) return;
        variableList.clear();
        serializeInner(head);
	return;
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

void proposition::assign(const std::vector<bool> &assignments) {
        auto listIt = variableList.begin();
        auto assignIt = assignments.begin();
        while (listIt != variableList.end() && assignIt != assignments.end()) {
                *(listIt)->truth = (*assignIt);
                *(listIt)->truthSet = true;
                ++listIt;
                ++assignIt;
        }
        if (listIt != variableList.end()) {
                std::cout << "Warning: Assignments list too small - some variables u
nassigned" << std::endl;
        }
        if (assignIt != assignments.end()) {
                std::cout << "Warning: Assignments list too large - not all values a
re used" << std::endl;
        }
        return;
}

bool proposition::eval(const std::vector<bool> &assignments) {
        serializeVariables();
        populateGraph(assignment); // handle assignments
        return evalInner(getHead());
}


bool evalInner(node *curr) {
        if (curr == NULL) return true;
        else if (curr->type == VARIABLE) {
                if (connective != "NOT") return curr->truth;
                else return !currNorth->truth;
        } else if (curr->type == LITERAL) {
                return (curr->truth) // T/F _must_ not be negated, but the value instead should be.
        } else if (type == UNARY) {
                if (connective == "NOT") {
                        return !evalInner(curr->lChild);
                }
                else { // Unknown behaviour, assume it doesn't change anything
                        return evalInner(curr->lChild)
                }

        }
        else if (type == BINARY) {
                if (connective == "AND")                return (evalInner(curr->lChild) && evalInner(curr->rChild);
                else if (connective == "OR")            return (evalInner(curr->lChild) || evalInner(curr->lChild));
                else if (connective == "IMPLIES")       return (!evalInner(curr->lChild) || evalInner(curr->rChild));
                else if (connective == "IFF")           return ((!evalInner(curr->lChild) && !evalInner(curr->rChild)) ||
                                                                (evalInner(curr->lChild) && evalInner(curr->rChild)));
                else if (connective == "XOR")           return (evalInner(curr->lChild) ^ evalInner(curr->rChild));
        }
        return false; // default to assuming it fails
}

proposition NNF(proposition P) {
        NNFInner(P->head);
        return P;
}

void NNFInner(node *currNode) {
        if (currNode->type = VARIABLE) return;
        else if (currNode->type = UNARY) {
                if (currNode->connective == "NOT") {
                        deMorgan(currNode->lChild);
                }
                NNFInner(currNode->lChild);
                return;
        } else {
                if (connective == "IMPLIES") {
                        impliesToNNF(currNode);
                } else if (connective == "IFF") {
                        iffToNNF(currNode);
                }
                NNFInner(currNode->lChild);
                NNFInner(currNode->rChild);
        }
        return;
}

void impliesToNNF(node *n) {
	node *left = new node(); // (P ^ Q)
        left->type = BINARY;
        left->lChild = n->lChild;
        left->rChild = n->rChild;
        left->connective = "AND";

        node *lNeg = new node(); // !P
        lNeg->type = UNARY;
        lNeg->lChild = n->lChild;
        lNeg->rChild = NULL;
        lNeg->connective = "NOT"

        node *rNeg = new node(); // !Q
	rNeg->type = UNARY;
        rNeg->lChild = n->rChild;
        rNeg->rChild = NULL;
        rNeg->connective = "NOT";

        node *right = new node(); // connecting the negations
        right->type = BIANRY;
        right->lChild = lNeg;
        right->rChild = rNeg;
        right->connective = "AND";

        n->lChild = left;
        n->rChild = right;
        n->connective = "OR";

        return;
}


void deMorgan(node *n) {
        if (n->type == UNARY && n->connective == "NEG") return;
        else if (n->type == OR) {

        }
}
               
}
