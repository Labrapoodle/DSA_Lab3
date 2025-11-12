#include <heaps.h>

fibheap *fibheap_create()
{
    fibheap *h = (fibheap *)malloc(sizeof(*h));
    if(h==NULL) return NULL;
    h->amount=0;
    h->min=NULL;
    return h;
}

fibnode *fibheap_insert(fibheap *heap,int key, char *value)
{
    fibnode *node = (fibnode *)malloc(sizeof(*node));
    node->key=key;
    node->value = value;
    node->left=node;
    node->right=node;
    node->parent=NULL;
    node->child=NULL;
    node->degree=0;
    node->mark=0;

    if(heap->min==NULL) heap->min = node;
    else
    {
        fibheap_add_to_rootList(node,heap->min);
        if(node->key < heap->min->key);
    }
    heap->amount++;
    return node; 
}

void fibheap_add_to_rootList(fibnode *toAdd, fibnode *currentMin)
{
    if(currentMin==NULL) return;

    // Случай 1: в списке корней только один элемент
    if(currentMin->left==currentMin)
    {
        currentMin->left = toAdd;
        currentMin->right = toAdd;
        toAdd->left = currentMin;
        toAdd->right = currentMin;
    }
    // Случай 2: в списке есть другие корни
    else
    {
        toAdd->left = currentMin->left;
        toAdd->right=currentMin;
        toAdd->left->right = toAdd;
        currentMin->left = toAdd;
    }
}

fibnode *fibheap_min(fibheap *heap)
{
    if(heap==NULL || heap->min==NULL) return NULL;
    return heap->min;
}

fibheap *fibheap_union(fibheap *h1,fibheap *h2)
{
    fibheap *H = fibheap_create();
    H->min = h1->min;
    fibheap_link_lists(h1->min,h2->min);
    if(h1->min==NULL || (h2->min!=NULL && h2->min->key < h1->min->key))
    {
        H->min = h2->min;
    }
    H->amount = h1->amount + h2->amount;

    free(h1);
    free(h2);
    return H;
}

void fibheap_link_lists(fibnode *min1, fibnode *min2)
{
    if(min1==NULL || min2==NULL) return;
    fibnode *right1 = min1->right;
    fibnode *left2 = min2->left;
    min1->right = min2;
    min2->left = min1;
    right1->left = left2;
    left2->right = right1;
}

fibnode *fibheap_extractmin(fibheap *heap)
{
    if(heap == NULL || heap->min == NULL) return NULL;
    fibnode *z = heap->min;
    if(z->child == NULL) return z; // Перерарботать потом
    fibnode *childIter = z->child;
    do
    {
        fibheap_add_to_rootList(childIter, heap->min);
        childIter->parent = NULL;
        childIter = childIter->right;
    }while(childIter->right != z->child);
    if(z == z->right) heap->min = NULL;
    else
    {
        heap->min = z->right;
        fibheap_consolidate(heap);
    }
    heap->amount--;
    return z;
}

void fibheap_remove_from_nodeList(fibheap *heap, fibnode *deletable)
{
    if(deletable->right == deletable) return;
    fibnode *left = deletable->left;
    fibnode *right = deletable->right;

    left->right = right;
    right->left = left;
}

void fibheap_consolidate(fibheap *heap)
{
    if(heap== NULL || heap->amount<1) return;
    int maxDegree = (int)log(heap->amount)/log(GOLDEN_RATIO) + 1;
    fibnode *degArray[maxDegree];
    for(int i=0; i<maxDegree; i++)
    {
        degArray[i]=NULL;
    }
    if(heap->min->right == heap->min) return; // Потом тоже исправить
    fibnode *nodeIterator = heap->min;
    while(nodeIterator != heap->min)
    {
        int nodeDegree = nodeIterator->degree;
        while(degArray[nodeDegree]!=NULL)
        {
            fibnode *y = degArray[nodeDegree];
            if(nodeIterator->key < y->key);
        }
    }

}

