#ifndef BRAUNSEQ_H
#define BRAUNSEQ_H

typedef double Data; /* defines the contained data type. Can be anything */

typedef struct bNode_ {
  Data el;
  struct bNode_* left;
  struct bNode_* right;
} bNode;

typedef bNode** Tree; /* tree is a place to store a ptr to the root bNode */

int size(Tree tree);
void addL(Tree tree, Data new_el); /* append to left  */
void addR(Tree tree, Data new_el); /* append to right */

Data remvL(Tree tree); /* remove from left, return removed element  */
Data remvR(Tree tree); /* remove from right, return removed element */

Data lookup(Tree tree, int i); /* return element at index i (or NULL) */

#endif

