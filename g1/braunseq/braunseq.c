#include <stdlib.h> // standard library, defining NULL, malloc, free
#include <stdio.h>  // standard I/O functions
#include "braunseq.h"

//#define DEBUG
#undef DEBUG


/*
 * internal helper functions
 *
 ****************************/
void error(char* msg) { // report an error and exit
    printf("PROGRAM ERROR: %s\n", msg);
    exit(EXIT_FAILURE);
}

// divide by 2 using bit shift
inline int half(int i) { return (i>>1); }
// test if an int is odd (return 1 if it is, 0 otherwise)
inline int odd(int i) { return (i & 0x1); }

// recursive helper for size: difference btw. tree size and m (recursive)
int diff(int n, bNode* t);
// insert at last position, knowing the tree size i >= 0 (for recursion)
void ins(Tree tree, int i, Data el);
// combine subtrees t1 and t2 into t1. Assumes size t1 same or 1 bigger than t2
void combine(Tree t1, Tree t2);
// delete last element, knowing the tree size i >= 0 (for recursion)
void del(Tree tree, int i);

int size(Tree tree) {
    if (*tree == NULL) {
        return 0;
    } else {
        int sz = size(&((*tree)->right));
        return (1 + 2*sz + diff(sz, (*tree)->left));
    }
}

void addL(Tree tree, Data new_el){
    // insert into new root, push old root into right subtree, swap sides
    
#ifdef DEBUG       
	printf("Adding note %d\n",(int)new_el);
#endif
    
    bNode *new = (bNode*)malloc(sizeof(bNode));
    new->el = new_el;
    new->left = NULL;
    new->right = NULL;    
    addLNode(tree,new);
    
#ifdef DEBUG    
	printf("node %d added\n",(int)new_el);
    printf("\tleft child: %p\n", (void*)new->left);
    printf("\tright child: %p\n", (void*)new->right);
#endif
    
    return;
}

void addLNode(Tree tree, bNode *node){
#ifdef DEBUG   
	printf("  push node %d to sub tree %p.\n",(int)(node->el),tree);
#endif
    
    bNode *oldRightTree, *oldLeftTree;
    
    
    
	if ((*tree) != NULL) {
    	oldRightTree = (*tree)->right;
    	oldLeftTree  = (*tree)->left;
        
        bNode *oldRoot = *tree;
        
        // Swithing the branches in this layer.
		node->right = oldLeftTree;        
		node->left = oldRightTree;
        
        // This node will be the new root.
		*tree = node;
                   
#ifdef DEBUG 
		printf("  Now branching \n");
#endif     
        // Calling addLNode recursively on the right subtree
        // of tree.        
	    addLNode(&node->left,oldRoot);
	 } else {
         
#ifdef DEBUG
         printf("  tree==null -> tree = &node\n");
#endif     
         
		*tree = node;
        node->left = NULL;
        node->right = NULL;
         
#ifdef DEBUG
        printf("\tnode %d added\n",(int)node->el);
        printf("\t\tleft child: %p\n", (void*)node->left);
        printf("\t\tright child: %p\n", (void*)node->right);
#endif     
         
	 }

}

void addR(Tree tree, Data new_el) {
    // navigate to last element (use size), insert there
    ins(tree, size(tree), new_el);
}

Data remvL(Tree tree) {
    // remove root (use "combine" helper), return removed element
    bNode old_root = **tree; // make a copy
    free(*tree); // return unused memory
    *tree = old_root.left;
    combine(tree, &old_root.right);
    return old_root.el;
}

Data remvR(Tree tree) {
    // use size to navigate through the tree recursively (helper del)
    // before deleting, call lookup to retrieve the data that is returned
    int treeSize = size(tree)-1;
    Data data = lookup(tree, treeSize);
    del(tree, treeSize);        
    return data;
}

Data lookup(Tree tree, int i) {
#ifdef DEBUG
    printf("din mor\n");
#endif
    // navigate through tree recursively (i odd/even)
    if (*tree == NULL || i < 0) {
        error("lookup: index out of bounds");
    }
    if (i == 0) {
        return (*tree)->el;
    }
    if (odd(i)) {
        return ( lookup(&((*tree)->left), half(i)) );
    } else {
        return ( lookup(&((*tree)->right), half(i)-1) );
    }
}

/* ************************ 
 * Implementation internals
 *
 **************************/

// helper for size: difference btw. tree size and m (recursive, O(log n))
int diff(int n, bNode* t) {
    if (n == 0) { return (t == NULL ? 0 : 1); }
    // n != 0
    if (t == NULL) { error("diff encountered an internal error."); }
    if (odd(n)) {
        return diff(half(n), t->left);
    } else {
        return diff(half(n)-1, t->right);
    }
}

// insert at last position, knowing size is i >= 0 (recursive)
void ins(Tree tree, int i, Data el) {
    if (i < 0) { error("ins received invalid data."); }
    if (i == 0) {
        bNode* node;
        if (*tree != NULL) { error("ins expected tree==NULL"); }
        node = malloc(sizeof(bNode));
        if (node == NULL) { error("insert: failed to malloc"); }
        node->el = el;
        node->left = NULL;
        node->right=NULL;
        *tree = node;
        return;
    }
    if (odd(i)) { // if i odd: insert into left subtree
        ins(&((*tree)->left), half(i), el);
    } else {      // if even: insert into right subtree
        ins(&((*tree)->right), half(i)-1, el);
    }
}

// combine two subtrees into one. assumes size t1 same or 1 bigger than t2
void combine(Tree t1, Tree t2) {
    if (*t1 == NULL) { return; // t1 empty, so is t2. Nothing to do.
    } else {
        // otherwise, the new root is the t1 root, with left subtrees t2 
        // and right subtree  combined from t1 subtrees (recursively)
        combine(&((*t1)->left), &((*t1)->right));
        (*t1)->right = (*t1)->left;
        (*t1)->left = *t2;
    }
}

// delete last element, knowing the size is i >= 0 (recursive)
void del(Tree tree, int i) {
    // navigate through tree recursively (i odd/even)
    if (*tree == NULL || i < 0) {
        error("lookup: index out of bounds");
    }
        
    if (i == 0) {        
        (*tree) = NULL;
        return;
    }    
    
    if (odd(i)) {
        del(&((*tree)->left), half(i));
    } else {
        del(&((*tree)->right), half(i)-1);
    }
    // navigate through the tree to find the last element (i is size).
    return;
}
