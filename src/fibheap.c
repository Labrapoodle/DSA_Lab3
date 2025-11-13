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
    if(heap==NULL) return;

    // Случай 0:
    if(heap->min == NULL)
    {
        heap->min = toAdd;
        toAdd->parent = NULL;
        toAdd->left = toAdd;
        toAdd->right = toAdd;
    } 

    // Случай 1: в списке корней только один элемент
    else if(heap->min->left == heap->min)
    {
        heap->min->left = toAdd;
        heap->min->right = toAdd;
        toAdd->left = heap->min;
        toAdd->right = heap->min;
    }
    // Случай 2: в списке есть другие корни
    else
    {
        toAdd->left = heap->min->left;
        toAdd->right=heap->min;
        toAdd->left->right = toAdd;
        heap->min->left = toAdd;
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
    if(z->child == NULL) return NULL; // Перерарботать потом
    fibnode *childIter = z->child;
    do
    {
        fibheap_add_to_rootList(childIter, heap);
        childIter->parent = NULL;
        childIter = childIter->right;
    }while(childIter != z->child);

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

/* Возможно вообще не нужна
void fibheapp_swap(fibnode *first, fibnode *second){
    fibnode temp = *first;
    if(first->right != first)
    {
        first->left->right = second;
        first->right->left = second;
    }    
    if(second->right != second)
    {
        second->left->right = first;
        second->right->left = first;
    }
    
    first->value = second->value;
    first->parent = second->parent;
    first->child = second->child;
    first->left = second->left;
    first->right = second->right;
    first->degree = second->degree;
    first->mark = second->mark;

    second->value  = temp.value;
    second->parent = temp.parent;
    second->child  = temp.child;
    second->left   = temp.left;
    second->right  = temp.right;
    second->degree = temp.degree;
    second->mark   = temp.mark;    
    


}
*/

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
    
    do
    {
        int nodeDegree = nodeIterator->degree;
        while(degArray[nodeDegree]!=NULL)
        {
            fibnode *y = degArray[nodeDegree];
            if(nodeIterator->key < y->key)
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
        nodeIterator = nodeIterator->right; 
    }while(nodeIterator != heap->min);
    heap->min = NULL;
    maxDegree = (int)log(heap->amount)/log(GOLDEN_RATIO) + 1;
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

    fibheap temp = {parent->child, 1};
    fibheap_add_to_rootList(child, &temp);
    child->mark = 0;
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
    fibheap temp_parent_childs = {parent->child, 1};
    fibheap_remove_from_nodeList( &temp_parent_childs, child);
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