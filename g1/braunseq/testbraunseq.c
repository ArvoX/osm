#include <stdlib.h>
#include <stdio.h>
#include "braunseq.h"

void printTree(Tree tree){
    int treeSize = size(tree);
    printf("%i\n", treeSize);
    for (int i=0; i<treeSize; i++) {
        printf("index %i = %f\n",i,lookup(tree, i));
    }
}


int main(){
    bNode *a = NULL;
    Tree tree=(&a);
    
    printf("instantiate tree..\n");
    printf("Adding elements\n");
    addL(tree,1);
    addL(tree,2);
    addL(tree,3);
    addL(tree,4);
    addL(tree,5);
    addL(tree,6);
    addL(tree,7);
    addL(tree,8);
    //printf("instantiate tree..\n");
    //printf("Size of tree = %d\n",size(tree));
    printTree(tree);
    
    printf("Now removing all elements from the tree\n");
    
    
    while (size(tree)) {
        printf("--remvR--\n");
        remvR(tree);
        printTree(tree);
    }
    
    
    return 0;
}
