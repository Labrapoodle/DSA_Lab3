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

    
    fibheap_add_to_rootList(node,heap);
    if(heap->min == NULL || key < heap->min->key) heap->min = node;
    
    heap->amount++;
    return node; 
}

void fibheap_add_to_rootList(fibnode *toAdd, fibheap *heap)
{
    if(heap==NULL || toAdd == NULL) return;

    toAdd->parent = NULL;

    // Случай 1: Куча пустая
    if(heap->min == NULL)
    {
        heap->min = toAdd;        
        toAdd->left = toAdd;
        toAdd->right = toAdd;
    }
    // Случай 2: Куча непустая    
    else
    {
        fibnode *min = heap->min;
        fibnode *leftOfMin = min->left; 

        toAdd->left = leftOfMin;
        toAdd->right = min;
        leftOfMin->right = toAdd;
        min->left = toAdd;
        
    }

    if(toAdd->key < heap->min->key) heap->min = toAdd;
    
}

fibnode *fibheap_min(fibheap *heap)
{
    if(heap==NULL || heap->min==NULL) return NULL;
    return heap->min;
}

fibheap *fibheap_union(fibheap *h1,fibheap *h2)
{
    if(h1 == NULL || h2 == NULL) return NULL;
    
    fibheap *H = fibheap_create();
    H->min = h1->min;
    if(h1->min != NULL && h2->min != NULL) fibheap_link_lists(h1->min,h2->min);
    
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
    if(heap->min != NULL) printf("min->key: %d; min-degree: %d; amount: %d; next: %d; last: %d;\n",
        heap->min->key,heap->min->degree,heap->amount,heap->min->right->key,heap->min->left->key);
    if(heap->min->child!=NULL) printf("\tmin-child: %d\n", heap->min->child->key);
    fibnode *z = heap->min;
    if(z->child != NULL)
    {
        
        fibnode *childIter = z->child;
        int iterationCounter = 0;
        do
        {
            
            fibnode *next = childIter->right;
            if(childIter == next)
            {
                
                z->child = NULL;
                
            } 
            else 
            {
                z->child = next;
                
                next->left = childIter->left;
                next->left->right = next;
            }
            
            
            
            
            fibheap_add_to_rootList(childIter, heap);
            childIter->parent = NULL;
            
            
            childIter = next;
            iterationCounter++;
            
            
        }while(z->child != NULL);
    }
    
    fibheap_remove_from_nodeList(heap, z);
    
    
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
    if(deletable->right == deletable) 
    {
        if(heap->min == deletable)
        {
            heap->min = NULL;
        }
        return;
    }
    fibnode *left = deletable->left;
    fibnode *right = deletable->right;
    
    left->right = right;
    right->left = left;
    
    
    
    if(heap->min == deletable) heap->min = right;
    
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
    
    if(heap->min == NULL) return;
    if(heap->min->right == heap->min) return;
    
    fibnode *nodeIterator = heap->min;
    int firstIterationFlag = 1;
    
    do
    {
        printf("\033[31mz->key: jopa , heap->min: %d\033[0m\n", heap->min->key);
        fibnode *next = nodeIterator->right;
        int nodeDegree = nodeIterator->degree;
        
        while(nodeDegree<maxDegree && degArray[nodeDegree]!=NULL)
        {
            
            fibnode *y = degArray[nodeDegree];
            if(nodeIterator->key > y->key)
            {                
                fibnode *temp = nodeIterator;
                nodeIterator = y;
                y = temp;
            }
            
            fibheap_link(heap, y, nodeIterator);
            degArray[nodeDegree] = NULL;
            nodeDegree++;
        }
        degArray[nodeDegree] = nodeIterator;
        nodeIterator = next;
        firstIterationFlag =0;
    }while(nodeIterator != heap->min  || firstIterationFlag );
    
    

    heap->min = NULL;   
    
    
    for(int i=0; i<maxDegree; i++)
    {
        if(degArray[i] != NULL)
        {

            fibheap_add_to_rootList(degArray[i],heap);
            if(heap->min == NULL ||
                degArray[i]->key < heap->min->key)
            {
                heap->min = degArray[i];
            }
        }
    }

}

void fibheap_link(fibheap *heap, fibnode *child, fibnode *parent)
{
    parent->degree++;
    fibheap_remove_from_nodeList(heap, child);
    child->parent = parent;
    child->mark = 0;

    if(parent->child == NULL)
    {
        parent->child = child;
        child->left = child;
        child->right = child;
    }
    else{
        fibnode *existingChild = parent->child;
        fibnode *existingChildLeft = existingChild->left;

        child->left = existingChildLeft;
        child->right = existingChild;
        existingChildLeft->right = child;
        existingChild->left = child;

        if(child->key < existingChild->key) parent->child = child;
    }
    
}

fibnode *fibheap_decrease_key(fibheap *heap,fibnode *node, int newkey)
{
    if(newkey > node->key) return NULL;
    node->key = newkey;
    fibnode *parent = node->parent;
    if( parent != NULL &&
            node->key < parent->key)
    {
        fibheap_cut(heap, node, parent);
        fibheap_cascading_cut(heap, parent);
    }
    if(node->key < heap->min->key) heap->min = node;
    return node;
}

void fibheap_cut(fibheap *heap, fibnode *child, fibnode *parent)
{
    if(heap == NULL || child == NULL || parent == NULL) return;
    fibnode *childLeft = child->left;
    fibnode *childRight = child->right;

    
    childLeft->right = childRight;
    childRight->left = childLeft;    
    
    
    if(parent->child == child)
    {
        if(child->right == child) parent->child = NULL;
        else parent->child = childRight;
    }

    parent->degree--;
    fibheap_add_to_rootList(child, heap);
    child->parent = NULL;
    child->mark = 0;
}

void fibheap_cascading_cut(fibheap *heap, fibnode *node)
{
    if(node == NULL || heap == NULL || node->parent == NULL) return;
    fibnode *z = node->parent;
    if(node->mark == 0) node->mark = 1;
    else
    {
        fibheap_cut(heap, node, z);
        fibheap_cascading_cut(heap,z);
    }
}

fibnode *fibheap_lookup(fibnode *init, int key)
{
    if( init == NULL) return NULL;
    fibnode *nodeIter = init;
    do
    {
        if(nodeIter->key == key) return nodeIter;
        if(nodeIter->child != NULL)
        {
            fibnode *res = fibheap_lookup(nodeIter->child, key);
            if(res != NULL) return res;
        }
        
        nodeIter = nodeIter->right;
    }while(nodeIter != init);
    return NULL;
}

fibnode *fibheap_delete(fibheap *heap, int key)
{
    fibnode *x = fibheap_lookup(heap->min, key);
    if(x == NULL) return NULL; 
    fibheap_decrease_key(heap, x, INT_MIN);
    fibheap_extractmin(heap);
    return x;
}

void fibheap_free(fibnode *node)
{
    if(node == NULL ) return;
    fibnode *nodeIter = node;
    node->left->right = NULL;
    
    while(nodeIter != NULL)
    {
        fibnode *next = nodeIter->right;
        if(nodeIter->child != NULL) fibheap_free(nodeIter->child);     
        free(nodeIter);
        nodeIter = next;
    }
    
    
    
}
//malloc