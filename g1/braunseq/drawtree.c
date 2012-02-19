#include <stdio.h>
#include "braunseq.h"
#include "queue.h"
const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
void draw(bNode *node, int i);
void fun(Tree tree);
int height(Tree tree);
int wide(int h);

void drawtree(Tree tree)
{
//        int size = Size(tree);
//        draw(*tree, 0);
        fun(tree);
}

void fun(Tree tree)
{
        Queue* queue = queue_alloc();
        queue_add(queue, *tree);
        int h = height(tree);
#ifdef DEBUG
        printf("h: %i\n", h);
#endif
        for(int l = 0; l < h; l++)
        {
                int c = h-l;
                int w = wide(c);
                int b = w/4;
                int s = b + 1;
                if(c == 1)
                        s = 0;
		else if(c > 2)
			s++;
                int u = b - 1;
                if(u < 0)
                        u = 0;
#ifdef DEBUG
                printf("c: %i ", c);
                printf("w: %i ", w);
                printf("b: %i ", b);
                printf("s: %i ", s);
                printf("u: %i\n", u);
#endif
                for(int r = 0; r < (1 << l); r++)
                {
                        for(int i = 0; i < s; i++)
                                printf(" ");
                        for(int i = 0; i < u; i++)
                                printf("_");

                        bNode* node = queue_pop(queue);
			if(node != NULL)
			{
                                queue_add(queue, node->left);
                                queue_add(queue, node->right);
			        printf("%c", b64[((int)node->el) % 64]);
                        }
                        else
			        printf("!");

                        for(int i = 0; i < u; i++)
                                printf("_");
                        int e = s + 1;
                        if(c == 1 && !(r & 1))
                                e = 3;
                        for(int i = 0; i < e; i++)
                                printf(" ");
                }
                printf("\n");
                if(c == 1)
                        continue;
                
                for(int r = 0; r < (1 << l); r++)
                {
			for(int i = 0; i < s-1; i++)
				printf(" ");
                        printf("/");
                        for(int i = 0; i < 2 * u + 1; i++)
                                printf(" ");
                        printf("\\");
                        for(int i = 0; i < s; i++)
                                printf(" ");
                }
                printf("\n");
        }
        queue_free(queue);
}

/*
______________________
                         ______________________X______________________
__________              /                                             \
             __________X__________                           __________X__________
____        /                     \                         /                     \
       ____X____               ____X____               ____X____               ____X____
_     /         \             /         \             /         \             /         \
    _X_         _X_         _X_         _X_         _X_         _X_         _X_         _X_
   /   \       /   \       /   \       /   \       /   \       /   \       /   \       /   \
  X     X     X     X     X     X     X     X     X     X     X     X     X     X     X     X
 / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \
X   X X   X X   X X   X X   X X   X X   X X   X X   X X   X X   X X   X X   X X   X X   X X   X
}
1   2   3   4   5   6 = h
2  *2  *2  *2  *2  *2 
   +1  +1  +1  +1  +1

2   5  11  23  47  95 = 3*2^(h-1)-1 = b
space = floor(b/4)+1
underscore = floor(b/4)-2

0   2   4   6   8  10 = 2*(’-1)
           +1  +1  +1
               +4  +4
                  +10
*/
/*
25                         ______________________0______________________
24                        /                                             \
13             __________1__________                           __________2__________
12            /                     \                         /                     \
7        ____3____               ____5____               ____4____               ____6____
6       /         \             /         \             /         \             /         \
4     _7_         11_         _9_         13_         _8_         12_         10_         14_
3    /   \       /   \       /   \       /   \       /   \       /   \       /   \       /   \
2   15   23     19   27     17   25     21   29     16   24     20   28     18   26     22   30
1  / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \
0 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ??
*/
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

int height(Tree tree)
{
        if(tree == NULL)
                return -1;
        int c = 0;
        for(bNode *node = *tree; node != NULL; node = node->left)
                c++;
        return c;
}
int wide(int h)
{
        //https://oeis.org/A153893
        //3*2^(n)-1
        return  (3 << (h-1)) - 1;
}
/*
        1 2  3  4   5    6
        
        0 0  1  4  10   22
                 *2+2
        3 5 11 23  47
              *2+1
        0 2  4  7  13   25
                 *2-1
*/
