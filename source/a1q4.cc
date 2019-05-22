/*
 * Still assumes it uses the same data structure from before.
 *
 */

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
	node *tmp = new node();
	tmp->type = UNARY;
	tmp->varName = "NOT";
	tmp->lChild = n->lChild;
	n->lChild = tmp;
	n->varName = "OR";	
	return;
}

void iffToNNF(node *n) { // P <--> Q becomes (P ^ Q) v (!P ^ ! Q)
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
