#include <stdio.h>
#include "braunseq.h"

void draw(bNode *node, int i);

void drawtree(Tree tree)
{
//	int size = Size(tree);
	draw(*tree, 0);

/*
//    _0_
//   /   \
//  1     2
// / \   / \
//4   6 5   7
*/
}

void draw(bNode *node, int i)
{
	for(int j = i; j > 0; j--)
		if(j == 1)
			printf("+-");
		else
			printf("  ");
	if(node == NULL)
		printf("null\n");
	else
	{
		printf("%f\n", node->el);
		draw(node->left, i + 1);
		draw(node->right, i + 1);
	}
}
/*
//0
//	1
//		4
//		6
//	2
//		5
//		7
*/
