#include <stdio.h>
#include <assert.h>
#include "heaps.h"

void test_create_and_insert() {
    printf("Test 1: Create and Insert\n");
    fibheap *heap = fibheap_create();
    assert(heap != NULL);
    assert(heap->min == NULL);
    assert(heap->amount == 0);
    
    fibnode *node1 = fibheap_insert(heap, 10, "node10");
    assert(node1 != NULL);
    assert(heap->min == node1);
    assert(heap->amount == 1);
    assert(node1->key == 10);
    
    fibnode *node2 = fibheap_insert(heap, 5, "node5");
    assert(heap->min == node2);
    assert(heap->amount == 2);
    
    fibheap_insert(heap, 15, "node15");
    //fibnode *node3 = fibheap_insert(heap, 15, "node15");
    assert(heap->min == node2);
    assert(heap->amount == 3);

    fibheap_free(heap->min);
    free(heap);
    
    printf("✓ Create and Insert passed\n");
}

void test_min_operation() {
    printf("Test 2: Min Operation\n");
    fibheap *heap = fibheap_create();
    
    assert(fibheap_min(heap) == NULL);
    
    fibheap_insert(heap, 20, "node20");
    fibnode *min_node = fibheap_min(heap);
    assert(min_node != NULL);
    assert(min_node->key == 20);
    
    fibheap_insert(heap, 5, "node5");
    min_node = fibheap_min(heap);
    assert(min_node->key == 5);
    
    fibheap_insert(heap, 3, "node3");
    min_node = fibheap_min(heap);
    assert(min_node->key == 3);
    
    fibheap_free(heap->min);
    free(heap);

    printf("✓ Min Operation passed\n");
}

void test_extract_min() {
    printf("Test 3: Extract Min\n");
    fibheap *heap = fibheap_create();
    
    fibheap_insert(heap, 10, "node10");
    fibheap_insert(heap, 5, "node5");
    fibheap_insert(heap, 15, "node15");
    fibheap_insert(heap, 3, "node3");
    fibheap_insert(heap, 7, "node7");
    
    assert(heap->amount == 5);
    
    fibnode *min = fibheap_extractmin(heap);
    assert(min != NULL);
    assert(min->key == 3);
    assert(heap->amount == 4);
    assert(heap->min->key == 5);
    free(min);
    
    min = fibheap_extractmin(heap);
    assert(min->key == 5);
    assert(heap->amount == 3);
    assert(heap->min->key == 7);
    free(min);
    
    min = fibheap_extractmin(heap);
    assert(min->key == 7);
    assert(heap->amount == 2);
    assert(heap->min->key == 10);
    free(min);
    
    fibheap_free(heap->min);
    free(heap);

    printf("✓ Extract Min passed\n");
}

void test_decrease_key() {
    printf("Test 4: Decrease Key\n");
    fibheap *heap = fibheap_create();
    
    fibnode *node20 = fibheap_insert(heap, 20, "node20");
    fibnode *node10 = fibheap_insert(heap, 10, "node10");
    fibnode *node15 = fibheap_insert(heap, 15, "node15");
    
    assert(heap->min->key == 10);
    
    // Уменьшаем ключ у node20
    fibheap_decrease_key(heap, node20, 5);
    assert(heap->min->key == 5);
    assert(heap->min == node20);
    
    // Уменьшаем ключ у node15
    fibheap_decrease_key(heap, node15, 3);
    assert(heap->min->key == 3);
    assert(heap->min == node15);
    
    // Попытка увеличить ключ (не должна сработать)
    fibnode *result = fibheap_decrease_key(heap, node10, 25);
    assert(result == NULL);
    assert(node10->key == 10);
    
    fibheap_free(heap->min);
    free(heap);

    printf("✓ Decrease Key passed\n");
}

void test_delete() {
    printf("Test 5: Delete\n");
    fibheap *heap = fibheap_create();
    
    fibheap_insert(heap, 10, "node10");
    fibheap_insert(heap, 5, "node5");
    fibheap_insert(heap, 15, "node15");
    //fibnode *node15 = fibheap_insert(heap, 15, "node15");
    fibheap_insert(heap, 3, "node3");
    fibheap_insert(heap, 7, "node7");
    
    assert(heap->amount == 5);
    
    // Удаляем существующий элемент
    fibnode *deleted = fibheap_delete(heap, 15);
    assert(deleted != NULL);
    assert(heap->amount == 4);
    
    // Проверяем, что элемент действительно удален
    fibnode *lookup_result = fibheap_lookup(heap->min, 15);
    assert(lookup_result == NULL);
    free(deleted);
    // Пытаемся удалить несуществующий элемент
    deleted = fibheap_delete(heap, 999);
    assert(deleted == NULL);
    assert(heap->amount == 4);
    
    fibheap_free(heap->min);
    free(heap);

    printf("✓ Delete passed\n");
}

void test_union() {
    printf("Test 6: Union\n");
    fibheap *heap1 = fibheap_create();
    fibheap *heap2 = fibheap_create();
    
    fibheap_insert(heap1, 10, "h1_10");
    fibheap_insert(heap1, 5, "h1_5");
    fibheap_insert(heap2, 3, "h2_3");
    fibheap_insert(heap2, 7, "h2_7");
    
    fibheap *merged = fibheap_union(heap1, heap2);
    assert(merged != NULL);
    assert(merged->amount == 4);
    assert(merged->min->key == 3);
    
    fibheap_free(merged->min);
    free(merged);



    printf("✓ Union passed\n");
}

void test_edge_cases() {
    printf("Test 7: Edge Cases\n");
    
    // Тест с пустой кучей
    fibheap *empty_heap = fibheap_create();
    assert(fibheap_extractmin(empty_heap) == NULL);
    assert(fibheap_min(empty_heap) == NULL);
    fibheap_free(empty_heap->min);
    free(empty_heap);
    // Тест с одним элементом
    fibheap *single_heap = fibheap_create();
    fibnode *single = fibheap_insert(single_heap, 42, "single");
    assert(single_heap->min == single);
    assert(single_heap->amount == 1);
    
    fibnode *extracted = fibheap_extractmin(single_heap);
    assert(extracted == single);
    assert(single_heap->min == NULL);
    assert(single_heap->amount == 0);
    
    // Тест lookup в пустой куче
    assert(fibheap_lookup(NULL, 10) == NULL);
    free(extracted);
    fibheap_free(single_heap->min);
    free(single_heap);
    printf("✓ Edge Cases passed\n");
}

void test_complex_scenario() {
    printf("Test 8: Complex Scenario\n");
    fibheap *heap = fibheap_create();
    
    // Вставляем элементы в разном порядке
    fibnode *nodes[10];
    int keys[] = {50, 20, 80, 10, 30, 60, 90, 5, 15, 25};
    
    for (int i = 0; i < 10; i++) {
        char value[20];
        sprintf(value, "node%d", keys[i]);
        nodes[i] = fibheap_insert(heap, keys[i], value);
    }
    
    assert(heap->amount == 10);
    assert(heap->min->key == 5);
    
    // Извлекаем минимальные элементы
    for (int i = 0; i < 5; i++) {
        fibnode *min = fibheap_extractmin(heap);
        assert(min != NULL);
        free(min);
    }
    
    assert(heap->amount == 5);
    
    // Уменьшаем ключи
    fibheap_decrease_key(heap, nodes[2], 2); // node80 -> 2
    assert(heap->min->key == 2);
    
    // Удаляем элемент
    fibnode *deleted = fibheap_delete(heap, 30);
    free(deleted);
    assert(heap->amount == 4);
    fibheap_free(heap->min);
    free(heap);
    printf("✓ Complex Scenario passed\n");
}

void test_memory_management() {
    printf("Test 9: Memory Management\n");
    
    fibheap *heap = fibheap_create();
    
    // Создаем несколько узлов
    for (int i = 0; i < 100; i++) {
        char value[20];
        sprintf(value, "test%d", i);
        fibheap_insert(heap, i * 10, value);
    }
    
    assert(heap->amount == 100);
    
    // Извлекаем все элементы
    while (heap->amount > 0) {
        //if(heap->amount == 3) fibheap_print_basic(heap); 
        fibnode *min = fibheap_extractmin(heap);
        
        assert(min != NULL);
        free(min);
    }
    assert(heap->min == NULL);
    assert(heap->amount == 0);
    fibheap_free(heap->min);
    free(heap);
    printf("✓ Memory Management passed\n");
} 

void run_all_tests() {
    printf("Starting Fibonacci Heap Tests...\n\n");
    
    test_create_and_insert();
    test_min_operation();
    test_extract_min();
    test_decrease_key();    
    test_delete();
    test_union();    
    test_edge_cases();    
    test_complex_scenario();    
    test_memory_management();
    
    printf("\n🎉 All tests passed successfully!\n");
}

int main() {
    run_all_tests();
    return 0;
}