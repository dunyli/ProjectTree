#define _CRT_SECURE_NO_WARNINGS

#include "map.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Создание пустого словаря
 */
Map* map_create(void) {
    Map* map = (Map*)malloc(sizeof(Map));
    if (!map) return NULL;

    map->tree = rb_create();
    if (!map->tree) {
        free(map);
        return NULL;
    }

    return map;
}

/*
 * Удаление словаря и освобождение памяти
 */
void map_destroy(Map* map) {
    if (!map) return;

    rb_destroy(map->tree);
    free(map);
}

/*
 * Вставка пары (ключ, значение) в словарь
 * Если ключ уже существует, обновляет значение
 * Сложность: O(log n)
 */
void map_put(Map* map, const char* key, int value) {
    rb_insert(map->tree, key, value);
}

/*
 * Получение значения по ключу
 * Возвращает true если ключ найден, false если нет
 * Сложность: O(log n)
 */
bool map_get(Map* map, const char* key, int* out_value) {
    return rb_get(map->tree, key, out_value);
}

/*
 * Проверка наличия ключа в словаре
 * Сложность: O(log n)
 */
bool map_contains(Map* map, const char* key) {
    return rb_contains(map->tree, key);
}

/*
 * Удаление ключа из словаря
 * Возвращает true если ключ найден и удалён, иначе false
 * Сложность: O(log n)
 */
bool map_remove(Map* map, const char* key) {
    return rb_delete(map->tree, key);
}

/*
 * Получение количества элементов в словаре
 * Сложность: O(1)
 */
int map_size(Map* map) {
    return rb_size(map->tree);
}

/*
 * Проверка, пуст ли словарь
 * Сложность: O(1)
 */
bool map_is_empty(Map* map) {
    return map_size(map) == 0;
}

/*
 * Очистка словаря (удаление всех элементов)
 * Сложность: O(n)
 */
void map_clear(Map* map) {
    rb_destroy(map->tree);
    map->tree = rb_create();
}

/*
 * Итерация по всем элементам словаря
 * Для каждого элемента вызывается callback
 */
void map_for_each(Map* map, void (*callback)(const char* key, int value)) {
    rb_inorder(map->tree, callback);
}

/*
 * Печать содержимого словаря
 */
void map_print(Map* map) {
    printf("Словарь (size=%d): {", map_size(map));

    bool first = true;
    map_for_each(map, (void (*)(const char*, int))(void*)({
        if (!first) printf(", ");
        printf("\"%s\":%d", key, value);
        first = false;
        }));

    printf("}\n");
}