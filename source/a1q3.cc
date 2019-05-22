/*
 * For assignment 1 Question 3. 
 * It's assumed that the assignment is a mapping (akin to std::map<node*, bool>)
 * However if we were just provided physical copies of the assignments, 
 * an inorder traversal where we set the truth values can also suffice.
 * 
 * However, this is not particularly important for the question and as such a strong 
 * assumption is made. 
 */

#include <map>


/* 
 * populateGraph() will just semantically assign the truth values to every node provided
 * in the mapping for the purpose of this example. 
 * 
 * This assumes that the map is well formed (ie all node pointers are pointing to valid variables),
 * and that it will include every node required. 
 * 
 * In pratice, more validation would be required.
 */
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
		std::cout << "Warning: Assignments list too small - some variables unassigned" << std::endl;
	}
	if (assignIt != assignments.end()) {
		std::cout << "Warning: Assignments list too large - not all values are used" << std::endl;
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
		if (connective == "AND") 		return (evalInner(curr->lChild) && evalInner(curr->rChild);
		else if (connective == "OR")		return (evalInner(curr->lChild) || evalInner(curr->lChild));
		else if (connective == "IMPLIES") 	return (!evalInner(curr->lChild) || evalInner(curr->rChild));
		else if (connective == "IFF")		return ((!evalInner(curr->lChild) && !evalInner(curr->rChild)) ||
								(evalInner(curr->lChild) && evalInner(curr->rChild)));
		else if (connective == "XOR") 		return (evalInner(curr->lChild) ^ evalInner(curr->rChild));
	}
	return false; // default to assuming it fails
}
