#include <stdlib.h>
#include <stdio.h>
#include "braunseq.h"
#include "drawtree.h"

void printaslist(Tree tree){
    int s = size(tree);
    printf("[ ");
    for(int i = 0; i < s; i++){
        printf("%i ",(int)lookup(tree,i));        
    }
    printf("]\n");
}

int main(){
    bNode *a = NULL;
    Tree tree=(&a);
 /*   
    printf("instantiate tree..\n");
    printf("Adding elements using addL\n");
    for(int i = 10;i >= 0; i--)
        addL(tree,i);
    printf("Tree:\n");
    drawtree(tree);
    
    printf("Now removing all elements from the tree\n");
    
    while (size(tree)) {
        printf("--remvR--\n");
        remvR(tree);
        printf("Tree:\n");
        drawtree(tree);
    }
*/   

    printf("--------------------------\n");
    printf("Adding elements using addL\n");
    printf("--------------------------\n");
    for(int i = 0;i < 10; i++){        
        printf("Adding %i using addL\n",i);
        addL(tree,i);
        printf("Tree:\n");
        printaslist(tree);
        drawtree(tree);
        printf("\n\n");
    }
    
    printf("--------------------------\n");
    printf("    Testing lookup        \n");
    printf("--------------------------\n");
    printf("Current tree as list:\n");
    printaslist(tree);
    printf("lookup: tree[0] == %i\n",(int)lookup(tree,0));
    printf("lookup: tree[3] == %i\n",(int)lookup(tree,3));
    printf("lookup: tree[9] == %i\n",(int)lookup(tree,9));
    
    
    printf("-------------------------------\n");
    printf("Remove all elements using remvR\n");
    printf("-------------------------------\n");
    while (size(tree)) {
     remvR(tree);
     printf("Tree:\n");
     printaslist(tree);
     drawtree(tree);
     printf("\n\n");
    }
    
    printf("--------------------------\n");
    printf("Adding elements using addR\n");
    printf("--------------------------\n");
    for(int i = 0;i < 10; i++){        
        printf("Adding %i using addR\n",i);
        addR(tree,i);
        printf("Tree:\n");
        printaslist(tree);
        drawtree(tree);
        printf("\n\n");
    }
    
    printf("-------------------------------\n");
    printf("Remove all elements using remvL\n");
    printf("-------------------------------\n");
    while (size(tree)) {
        remvL(tree);
        printf("Tree:\n");
        printaslist(tree);
        drawtree(tree);
        printf("\n\n");
    } 
    
    printf("-------------------------------\n");
    printf("Try removing an elements from a\n");
    printf("empty tree.                    \n");
    printf("-------------------------------\n");
    remvL(tree);
    printf("Tree:\n");
    printaslist(tree);
    drawtree(tree);
    printf("\n\n");
    return 0;
}
