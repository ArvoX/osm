#include <stdlib.h>
#include <stdio.h>
#include "braunseq.h"

int main(){
	Tree tree;
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
	//printf("instantiate tree..\n");
	//printf("instantiate tree..\n");
	printf("Size of tree = %d\n",size(tree));

	return 0;
}
