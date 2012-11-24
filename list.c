#include<stdlib.h>
#include<stdio.h>
#include "list.h"


static List* create_list();
static int destroy_list(List *l);

static int insert_front(List *l, void *data);
static int delete_front(List *l, void **data);
static int insert_end(List *l, void* data);


int get_length(List *l){
	if (!l)
		return -1;
	else
		return l->len;
}

int get_type(List *l){
	if (!l)
		return -1;
	else
		return l->type;
}

int remove_node(List* l, void* data){
	
	if(l==NULL) return -1;

	Node* curr_node = l->head;
	if(curr_node==NULL) return -1;
	
	Node* prev_node = NULL;
	Node* temp_node;	
	
	while( curr_node ){
		temp_node = curr_node;

		if ( data == curr_node->data){
			if( l->tail == curr_node)
				l->tail = prev_node;

			if( l->head == curr_node) 
				l->head = curr_node->next;	
			
			else
				prev_node->next = curr_node->next;
			
			free(temp_node);
			
			l->len --;
			
			return 0;
		}

		prev_node = curr_node;
		curr_node = curr_node->next;

	}

	return -1;
}

Queue* create_queue(){
	Queue* q = create_list();
	if (q) q->type = TYPE_QUEUE;
	return q;	
}

int destroy_queue(Queue *q){
	return destroy_list(q);
}

int enqueue(Queue *q, void *data){
	if (!q || q->type!= TYPE_QUEUE)
		return -1;
	else 
		return insert_end(q, data);
}

int dequeue(Queue *q, void** p_data){
	if (!q || q->type!= TYPE_QUEUE)
		return -1;
	else 
		return delete_front(q, p_data);
}

Stack* create_stack(){
	Stack* s = create_list();
	if (s) s->type = TYPE_STACK;
	return s;	
}

int destroy_stack(Stack *s){
	return destroy_list(s);
}

int push(Stack *s, void *data){
	if (!s || s->type!= TYPE_STACK)
		return -1;
	else 
		return insert_front(s, data);
}

int pop(Stack *s, void** p_data){
	if (!s || s->type!= TYPE_STACK)
		return -1;
	else 
		return delete_front(s, p_data);
}


static int insert_front(List *l, void* data){

	Node* n = (Node*) malloc(sizeof(Node));

	if (!n){
		fprintf(stderr, "insertion failed, cannot create node\n");
		return -1;
	}
	
	n->next = NULL;
	n->data = data;

	if (l->head == NULL)
		l->head = l->tail = n;
	else{
		n->next = l->head;
		l->head = n;
	}

	l->len++;

	return 0;
}


static int insert_end(List *l, void* data){

	Node* n = (Node*) malloc(sizeof(Node));

	if (!n){
		fprintf(stderr, "insertion failed, cannot create node\n");
		return -1;
	}
	
	n->next = NULL;
	n->data = data;

	if (l->head == NULL)
		l->head = l->tail = n;
	else{
		l->tail->next = n;
		l->tail = n;
	}

	l->len++;

	return 0;
}

static int delete_front(List *l, void **p_data){

	if (!l ){
		fprintf(stderr, "null list\n");
		return -1;
	}

	if (!p_data){
		fprintf(stderr, "null p_data\n");
		return -1;
	}

	if ( !(l->head) ){
    // fprintf(stderr, "deletion failed, empty list\n");
		return -1;
	}

	Node *n = l->head;

	if (l->head == l->tail)
		l->head = l->tail = NULL;
	else 
		l->head = l->head->next;

	*p_data = n->data;

	free(n);

	l->len--;

	return 0;

}

static List* create_list(){
	List *l = (List *)malloc(sizeof(List));
	if (!l) return NULL;
	l->head = l->tail = NULL;	
	l->len = 0;
	return l;
}

static int destroy_list(List *l){

	if (l->head != NULL){
		fprintf(stderr, "List not empty! Unable to destroy. Pop/Dequeue all nodes first!\n");
		return -1;
	}

	free(l);
	return 0;
}
