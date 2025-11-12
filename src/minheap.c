#include <heaps.h>

//исправить возврашаемые типы


minheap *minheap_create(int capacity)
{
    minheap *heap = (minheap *)malloc(sizeof(*heap));
    if(heap==NULL || capacity<0) return NULL;
    heap->capacity = capacity;
    heap->size = 0;
    heap->nodes = (heapnode *)malloc(capacity*sizeof(*heap->nodes));
    if(heap->nodes==NULL){
        free(heap);
        return NULL;
    }
    return heap;
}

heapnode *minheap_min(minheap *h)
{
    if(h == NULL || h->size==0) return NULL;
    return &(h->nodes[0]);
}

void heap_free(minheap *h)
{
    free(h->nodes);
    free(h);
}

void heap_swap(heapnode *a, heapnode *b)
{
    heapnode temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

heapnode *minheap_insert(minheap *h,int key, char *value)
{
    if(h->size==h->capacity) return NULL;
    h->nodes[h->size].key = key;
    h->nodes[h->size].value = value;    
    h->size++;
    int indToReturn = h->size-1;
    for(int i = h->size-1; i>0 && h->nodes[(i-1)/2].key > h->nodes[i].key; i = (i-1)/2)
    {
        heap_swap(&(h->nodes[(i-1)/2]),&(h->nodes[i]));
        indToReturn = i;
    }
    return &h->nodes[indToReturn];
}

heapnode minheap_extractmin(minheap *h)
{
    if(h->size==0) return (heapnode){-1, NULL};
    heapnode deletable = h->nodes[0];
    h->nodes[0] = h->nodes[--(h->size)];
    heap_fixup_down(h,0);
    return deletable;
}

void heap_fixup_down(minheap *h, int ind)
{
    while(1)
    {
        int left = 2*ind+1;
        int right = 2*ind+2;
        int smallest = ind;

        
        
        if(left < h->size && h->nodes[left].key<h->nodes[smallest].key) smallest= left;
        if(right < h->size && h->nodes[right].key<h->nodes[smallest].key) smallest = right;

        if(smallest==ind) break;
        heap_swap(&(h->nodes[ind]),&(h->nodes[smallest]));
        ind = smallest;
    }
}

heapnode *minheap_decrease_key(minheap *h,int key, int newkey)
{
    if(key<newkey) return NULL;
    int index = minheap_lookup(h,key);
    if(index == -1) return NULL;
    h->nodes[index].key = newkey;

    while(index>0 && h->nodes[index].key < h->nodes[(index-1)/2].key )
    {
        heap_swap(&(h->nodes[index]),&(h->nodes[(index-1)/2]));
        index = (index-1)/2;
    }
    return &h->nodes[index];
}

//Фу, O(n)
int minheap_lookup(minheap *h, int key)
{
    if(h->size == 0 || h==NULL) return -1;
    for (int i =0; i<h->size;i++)
    {
        if(h->nodes[i].key==key) return i;
    }
    return -1;
}

int minheap_delete(minheap *h, int key)
{
    if(h == NULL) return -1;
    int ind = minheap_lookup(h,key);
    if(ind == -1) return -1;

    h->nodes[ind] = h->nodes[--(h->size)];
    heap_fixup_down(h,ind);
    return 0;
}

void print_heap(minheap *h, const char *message) {
    printf("%s: ", message);
    if (h == NULL) {
        printf("NULL\n");
        return;
    }
    printf("[");
    for (int i = 0; i < h->size; i++) {
        printf("(%d:'%s')", h->nodes[i].key, h->nodes[i].value);
        if (i < h->size - 1) printf(", ");
    }
    printf("]\n");
}
