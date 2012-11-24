// General purpose queue and stack.
// based on linked list.  

#ifndef _LIST_H
#define _LIST_H


#define TYPE_LIST 0 //Interface not exposed 
#define TYPE_QUEUE 1
#define TYPE_STACK 2

typedef struct Node{
	void *data;
	struct Node *next;
}Node;

typedef struct List{
	Node *head;
	Node *tail;
	int len;
	int type;
}List;

typedef List Queue;
typedef List Stack;


int get_length(List *l);
int get_type(List *l);

// Remove the node pointed by n
int remove_node(List* l, void* data);

Queue *create_queue();
int destroy_queue(Queue *q);
int enqueue(Queue *q, void* data);
int dequeue(Queue *q, void** p_data);


Stack* create_stack();
int destroy_stack(Stack *s);
int push(Stack *q, void* data);
int pop(Stack *q, void** p_data);



#endif
