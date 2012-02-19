#include <stdlib.h>
#include "queue.h"

Queue* queue_alloc()
{
	return (Queue*)calloc(1,sizeof(Queue));
}
void queue_add(Queue* q, bNode* node)
{
	qNode* new = (qNode*)calloc(1,sizeof(qNode));
	new->node = node;
	if(q->first == NULL)
		q->first = new;
	else      
		q->last->next = new;
	q->last = new;
}

bNode* queue_pop(Queue* q)
{
	if(q->first == NULL)
		return NULL;

	qNode* first = q->first;
	bNode* data = q->first->node;
	q->first = q->first->next;
	if(q->first == NULL)
		q->last = NULL;
	free(first);
	return data;
}

void queue_free(Queue* q)
{
	qNode* cur;
	qNode* next = q->first;
	while(next != NULL)
	{
		cur = next;
		next = next->next;
		free(cur);
	}
	free(q);
}
