#include <heaps.h>

int main(){
    // Тест 1: Создание кучи
    printf("1. Создание кучи на 10 элементов:\n");
    minheap *heap = minheap_create(10);
    if (heap == NULL) {
        printf("ОШИБКА: Не удалось создать кучу!\n");
        return 1;
    }
    print_heap(heap, "Пустая куча");
    printf("\n");
    
    // Тест 2: Вставка элементов
    printf("2. Вставка элементов:\n");
    minheap_insert(heap, 30, "тридцать");
    minheap_insert(heap, 20, "двадцать");
    minheap_insert(heap, 10, "десять");
    minheap_insert(heap, 40, "сорок");
    minheap_insert(heap, 5, "пять");
    minheap_insert(heap, 25, "двадцать пять");
    print_heap(heap, "После вставки 6 элементов");
    
    heapnode *min = minheap_min(heap);
    if (min) {
        printf("Минимальный элемент: (%d:'%s')\n", min->key, min->value);
    }
    printf("\n");
    
    
    
    
    // Тест 5: Извлечение минимального
    printf("5. Извлечение минимальных элементов:\n");
    heapnode extracted;
    for (int i = 0; i < 3; i++) {
        extracted = minheap_extractmin(heap);
        if (extracted.value!=NULL && extracted.key!=-1) {
            printf("Извлечен: (%d:'%s')\n", extracted.key, extracted.value);
        } else {
            printf("Не удалось извлечь\n");
        }
        print_heap(heap, "Куча после извлечения");
    }
    printf("\n");
    
    
    
    
    
    min = minheap_min(heap);
    if (min) {
        printf("Минимальный элемент: (%d:'%s')\n", min->key, min->value);
    }
    printf("\n");
    
    
    
    // Очистка
    printf("11. Очистка памяти:\n");
    heap_free(heap);
    printf("Память освобождена\n");
    
    
    
    
    return 0;
}