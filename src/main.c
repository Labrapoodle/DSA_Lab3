#include "heaps.h"
#include <sys/time.h>
#define KEY_LOW_GAP 10000
#define KEY_HIGH_GAP 200000
#define AMOUNT_LEAST 50000
#define AMOUNT_HIGHEST 200000
#define AMOUNT_STEP 50000

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int getrand(int min, int max)
{
    return (double)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

int main()
{
    int addedKeysArray[KEY_HIGH_GAP+1];
    for (int i = 0; i<= KEY_HIGH_GAP; i++){
        addedKeysArray[i]=0;
    }
    //Table 1
    fibheap* fibHeap = fibheap_create();
    for(int i = AMOUNT_LEAST; i<AMOUNT_HIGHEST; i+=AMOUNT_STEP){
        minheap* minHeap = minheap_create(i);
        for(int j = 0; j < i; j++){
            int randN = getrand(KEY_LOW_GAP, KEY_HIGH_GAP); 
            minheap_insert(minHeap,randN,"m");
            fibheap_insert(fibHeap,randN,"f");
            if(addedKeysArray[randN]==0) addedKeysArray[randN] = 1;
        }

    }

    return 0;
}