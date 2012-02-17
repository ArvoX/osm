#include <stdlib.h>
#include <stdio.h>
#include "braunseq.h"
#include "drawtree.h"

int main(){
    bNode *a = NULL;
    Tree tree=(&a);
    
    printf("instantiate tree..\n");
    printf("Adding elements\n");
    for(int i = 15;i > 0; i--)
        addL(tree,i);
    drawtree(tree);
    
    printf("Now removing all elements from the tree\n");
    
    
    while (size(tree)) {
        printf("--remvR--\n");
        remvR(tree);
        drawtree(tree);
    }
   
    
    return 0;
}
