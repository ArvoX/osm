#include "braunseq.h"

typedef struct qNode_t
{
        bNode* node;
        struct qNode_t* next;
} qNode;

typedef struct queue_t
{
	qNode* first;
	qNode* last;
} Queue;

Queue* queue_alloc();
void queue_add(Queue* queue, bNode* node);
bNode* queue_pop(Queue* queue);
void queue_free(Queue* queue);
