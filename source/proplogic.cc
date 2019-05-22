#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include "proplogic.h"

namespace proplogic {
void NNFInner(node*);
void deMorgan(node*);
void impliesToNNF(node*);
void iffToNNF(node*);

node::node() {
        type = VARIABLE;
        connective = NIL;
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
        truth = truth_;
        truthSet = true;
        lChild = NULL;
        rChild = NULL;
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
	switch(connective) {
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
	const logicType currType = curr->getLogicType();
        if (currType == VARIABLE || currType == LITERAL) curr->printVar();
        else if (currType == UNARY) {
                curr->printConnective(); // Prints the connective name
                printInner(curr->getlChild()); // should be "Not"
        } else if (currType == BINARY) {
                printInner(curr->getlChild());
                curr->printConnective();
                printInner(curr->getrChild());
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
        if (curr == NULL) return;
        serializeInner(curr->getlChild());
        if (curr->getLogicType() == VARIABLE && !variableList.count(curr)) {
                variableList.insert(curr);
        }
        serializeInner(curr->getrChild());
        return;
}

void proposition::assign(const std::vector<bool> &assignments) {
        auto listIt = variableList.begin();
        auto assignIt = assignments.begin();
        while (listIt != variableList.end() && assignIt != assignments.end()) {
                (*(listIt))->setTruth(*assignIt);
                ++listIt;
                ++assignIt;
        }
        if (listIt != variableList.end()) {
                std::cout << "Warning: Assignments list too small - some variables unassigned" << std::endl;
        }
        if (assignIt != assignments.end()) {
                std::cout << "Warning: Assignments list too large - not all values are used" << std::endl;
        }
        return;
}

bool proposition::eval(const std::vector<bool> &assignments) {
        serializeVariables();
        assign(assignments); // handle assignments
        return evalInner(getHead());
}


bool evalInner(node *curr) {
        if (curr == NULL) return true;
        else if (curr->getLogicType() == VARIABLE) {
                if (curr->getConnectiveType() != NOT) return curr->getTruth();
                else return !curr->getTruth();
        } else if (curr->getLogicType() == LITERAL) {
                return (curr->getTruth()); // T/F _must_ not be negated, but the value instead should be.
        } else if (curr->getLogicType() == UNARY) {
                if (curr->getConnectiveType() == NOT) {
                        return !evalInner(curr->getlChild());
                }
                else { // Unknown behaviour, assume it doesn't change anything
                        return evalInner(curr->getlChild());
                }

        }
        else if (curr->getLogicType() == BINARY) {
                if (curr->getConnectiveType() == AND)                return (evalInner(curr->getlChild()) && evalInner(curr->getrChild()));
                else if (curr->getConnectiveType() == OR)            return (evalInner(curr->getlChild()) || evalInner(curr->getrChild()));
                else if (curr->getConnectiveType() == IMPLIES)       return (!evalInner(curr->getlChild()) || evalInner(curr->getrChild()));
                else if (curr->getConnectiveType() == IFF)           return ((!evalInner(curr->getlChild()) && !evalInner(curr->getrChild())) ||
                                                                (evalInner(curr->getlChild()) && evalInner(curr->getrChild())));
                else if (curr->getConnectiveType() == XOR)           return (evalInner(curr->getlChild()) ^ evalInner(curr->getrChild()));
        }
        return false; // default to assuming it fails
}

proposition NNF(proposition P) {
        NNFInner(P.getHead());
        return P;
}

void NNFInner(node *currNode) {
        if (currNode->getLogicType() == VARIABLE) return;
        else if (currNode->getLogicType() == UNARY) {
                if (currNode->getConnectiveType() == NOT) {
                        deMorgan(currNode->getlChild());
                }
                NNFInner(currNode->getlChild());
                return;
        } else {
                if (currNode->getConnectiveType() == IMPLIES) {
                        impliesToNNF(currNode);
                } else if (currNode->getConnectiveType() == IFF) {
                        iffToNNF(currNode);
                }
                NNFInner(currNode->getlChild());
                NNFInner(currNode->getrChild());
        }
        return;
}

void impliesToNNF(node *n) {
	node *left = new node(); // (P ^ Q)
        left->setLogicType(BINARY);
        left->setlChild(n->getlChild());
        left->setrChild(n->getrChild());
        left->setConnectiveType(AND);

        node *lNeg = new node(); // !P
        lNeg->setLogicType(UNARY);
        lNeg->setlChild(n->getlChild());
        lNeg->setrChild(NULL);
        lNeg->setConnectiveType(NOT);

        node *rNeg = new node(); // !Q
	rNeg->setLogicType(UNARY);
        rNeg->setlChild(n->getrChild());
        rNeg->setrChild(NULL);
        rNeg->setConnectiveType(NOT);

        node *right = new node(); // connecting the negations
        right->setLogicType(BINARY);
        right->setlChild(lNeg);
        right->setrChild(rNeg);
        right->setConnectiveType(AND);

        n->setlChild(left);
        n->setrChild(right);
        n->setConnectiveType(OR);

        return;
}


void deMorgan(node *n) {
        if (n->getLogicType() == UNARY && n->getConnectiveType() == NOT) return;
        else if (n->getConnectiveType() == OR) {

        }
}
               
}
