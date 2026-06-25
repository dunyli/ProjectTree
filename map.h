#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include "rbtree.h"

/*
 * Ассоциативный массив (словарь) на основе красно-чёрного дерева
 * Реализует операции: вставка, поиск, удаление, обновление
 * Все операции имеют сложность O(log n)
 */
typedef struct {
    RBTree* tree;               /* Внутреннее красно-чёрное дерево */
} Map;

/* Создание и удаление словаря */
Map* map_create(void);
void map_destroy(Map* map);

/* Основные операции */
void map_put(Map* map, const char* key, int value);
bool map_get(Map* map, const char* key, int* out_value);
bool map_contains(Map* map, const char* key);
bool map_remove(Map* map, const char* key);

/* Получение размера */
int map_size(Map* map);

/* Проверка на пустоту */
bool map_is_empty(Map* map);

/* Очистка словаря */
void map_clear(Map* map);

/* Итерация по всем элементам с передачей пользовательских данных */
void map_for_each(Map* map, void (*callback)(const char* key, int value, void* user_data), void* user_data);

/* Печать содержимого */
void map_print(Map* map);

#endif /* MAP_H */