#include <iostream>
#include <string>
#include <set>
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

}
