#ifndef HEAPS_H
#define HEAPS_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

const double GOLDEN_RATIO = 1.61803398875;


typedef struct{
    int key;
    char *value;
}heapnode;

typedef struct{
    int capacity;
    int size;
    heapnode *nodes;
}minheap;

typedef struct{
    int key;
    char *value;
    fibnode *parent;
    fibnode *child;   
    fibnode *left;
    fibnode *right;
    int degree;
    int mark;
}fibnode;

typedef struct{
    fibnode *min;
    int amount;
}fibheap;

//minheap:
int minheap_lookup(minheap *h, int key);
int minheap_delete(minheap *heap, int key);

void heap_free(minheap *h);
void heap_swap(heapnode *a, heapnode *b);
void heap_fixup_down(minheap *heap, int index);
void print_heap(minheap *h, const char *message);

minheap *minheap_create(int capacity);

heapnode *minheap_min(minheap *heap);
heapnode minheap_extractmin(minheap *heap);
heapnode *minheap_insert(minheap *heap,int key, char *value);
heapnode *minheap_decrease_key(minheap *heap,int key, int newkey);


//fibheap:
void fibheap_free(fibnode *min);
void fibheap_consolidate(fibheap *heap);
void fibheap_link_lists(fibnode *h1, fibnode *h2);
void fibheap_cascading_cut(fibheap *heap, fibnode *node);
void fibheap_add_to_rootList(fibnode *toAdd, fibheap *heap); 
void fibheap_cut(fibheap *heap, fibnode *child, fibnode *parent);
void fibheap_link(fibheap *heap, fibnode *child, fibnode *parent);
void fibheap_remove_from_nodeList(fibheap *heap, fibnode *deletable);

//void fibheapp_swap(fibnode *first, fibnode *second);

fibheap *fibheap_create();
fibheap *fibheap_union(fibheap *heap1,fibheap *heap2);

fibnode *fibheap_min(fibheap *heap);
fibnode *fibheap_extractmin(fibheap *heap);
fibnode *fibheap_delete(fibheap *heap, int key);
fibnode *fibheap_lookup(fibnode *init, int key);
fibnode *fibheap_insert(fibheap *heap,int key, char *value);
fibnode *fibheap_decrease_key(fibheap *heap,fibnode *node, int newkey);
#endif