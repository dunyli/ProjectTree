#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "rbtree.h"
#include "map.h"

/* Счётчики для итогов тестов */
static int passed = 0;
static int failed = 0;

/* Вспомогательная функция для проверки условий */
static void check(const char* description, int condition) {
    if (condition) {
        printf("Успешно: %s\n", description);
        passed++;
    }
    else {
        printf("Ошибка: %s\n", description);
        failed++;
    }
}

/* ============================================================================
 *                          ТЕСТ 1: КРАСНО-ЧЁРНОЕ ДЕРЕВО
 * ============================================================================ */

 /*
  * Тест 1.1: Создание и удаление дерева
  */
static void test_rb_create_destroy(void) {
    printf("\n--- Тест 1.1: Создание и удаление ---\n");

    RBTree* tree = rb_create();
    check("Дерево создано (не NULL)", tree != NULL);
    check("Размер пустого дерева = 0", rb_size(tree) == 0);
    check("Корень пустого дерева = NULL", tree->root == NULL);

    rb_destroy(tree);
    check("Дерево удалено без ошибок", 1);
}

/*
 * Тест 1.2: Вставка и поиск
 */
static void test_rb_insert_search(void) {
    printf("\n--- Тест 1.2: Вставка и поиск ---\n");

    RBTree* tree = rb_create();

    /* Вставляем элементы */
    rb_insert(tree, "apple", 10);
    check("Размер после вставки 1 элемента = 1", rb_size(tree) == 1);

    rb_insert(tree, "banana", 20);
    rb_insert(tree, "daniil", 30);
    rb_insert(tree, "date", 40);
    rb_insert(tree, "ivan", 50);
    check("Размер после вставки 5 элементов = 5", rb_size(tree) == 5);

    /* Проверяем поиск */
    int val;
    check("apple найден", rb_get(tree, "apple", &val) && val == 10);
    check("banana найден", rb_get(tree, "banana", &val) && val == 20);
    check("daniil найден", rb_get(tree, "daniil", &val) && val == 30);
    check("date найден", rb_get(tree, "date", &val) && val == 40);
    check("ivan найден", rb_get(tree, "ivan", &val) && val == 50);

    /* Проверяем отсутствующие ключи */
    check("fig не найден", !rb_contains(tree, "fig"));
    check("grape не найден", !rb_contains(tree, "grape"));

    /* Проверяем обновление */
    rb_insert(tree, "apple", 100);
    check("Обновление apple: размер не изменился (5)", rb_size(tree) == 5);
    check("apple теперь = 100", rb_get(tree, "apple", &val) && val == 100);

    rb_print(tree);
    rb_destroy(tree);
}

/*
 * Тест 1.3: Удаление
 */
static void test_rb_delete(void) {
    printf("\n--- Тест 1.3: Удаление ---\n");

    RBTree* tree = rb_create();

    /* Вставляем элементы */
    rb_insert(tree, "a", 1);
    rb_insert(tree, "b", 2);
    rb_insert(tree, "c", 3);
    rb_insert(tree, "d", 4);
    rb_insert(tree, "e", 5);
    rb_insert(tree, "f", 6);
    rb_insert(tree, "g", 7);

    check("Размер после вставки 7 элементов = 7", rb_size(tree) == 7);

    /* Удаляем элементы */
    check("Удаление c", rb_delete(tree, "c"));
    check("Размер после удаления = 6", rb_size(tree) == 6);
    check("c отсутствует", !rb_contains(tree, "c"));

    check("Удаление a", rb_delete(tree, "a"));
    check("Размер после удаления = 5", rb_size(tree) == 5);
    check("a отсутствует", !rb_contains(tree, "a"));

    check("Удаление g", rb_delete(tree, "g"));
    check("Размер после удаления = 4", rb_size(tree) == 4);
    check("g отсутствует", !rb_contains(tree, "g"));

    /* Проверяем, что остальные элементы на месте */
    int val;
    check("b всё ещё есть", rb_get(tree, "b", &val) && val == 2);
    check("d всё ещё есть", rb_get(tree, "d", &val) && val == 4);
    check("e всё ещё есть", rb_get(tree, "e", &val) && val == 5);
    check("f всё ещё есть", rb_get(tree, "f", &val) && val == 6);

    /* Повторное удаление */
    check("Повторное удаление c = false", !rb_delete(tree, "c"));
    check("Размер не изменился (4)", rb_size(tree) == 4);

    /* Удаление несуществующего */
    check("Удаление x = false", !rb_delete(tree, "x"));
    check("Размер не изменился (4)", rb_size(tree) == 4);

    rb_print(tree);
    rb_destroy(tree);
}

/*
 * Тест 1.4: Краевые случаи
 */
static void test_rb_edge_cases(void) {
    printf("\n--- Тест 1.4: Краевые случаи ---\n");

    /* Пустое дерево */
    RBTree* tree = rb_create();
    check("Удаление из пустого = false", !rb_delete(tree, "anything"));
    check("Поиск в пустом = false", !rb_contains(tree, "anything"));
    check("Размер пустого = 0", rb_size(tree) == 0);

    /* Один элемент */
    rb_insert(tree, "single", 42);
    check("Размер после вставки = 1", rb_size(tree) == 1);
    int val;
    check("single найден", rb_get(tree, "single", &val) && val == 42);

    /* Удаляем единственный элемент */
    check("Удаление single", rb_delete(tree, "single"));
    check("Размер после удаления = 0", rb_size(tree) == 0);
    check("single отсутствует", !rb_contains(tree, "single"));

    rb_destroy(tree);
}


/* ============================================================================
 *                          ТЕСТ 2: АССОЦИАТИВНЫЙ МАССИВ (СЛОВАРЬ)
 * ============================================================================ */

 /*
  * Тест 2.1: Создание и базовые операции
  */
static void test_map_basic(void) {
    printf("\n--- Тест 2.1: Создание и базовые операции ---\n");

    Map* map = map_create();
    check("Словарь создан (не NULL)", map != NULL);
    check("Размер = 0", map_size(map) == 0);
    check("Словарь пуст", map_is_empty(map) == true);

    map_destroy(map);
    check("Словарь удалён", 1);
}

/*
 * Тест 2.2: Вставка и поиск
 */
static void test_map_put_get(void) {
    printf("\n--- Тест 2.2: Вставка и поиск ---\n");

    Map* map = map_create();

    /* Вставляем элементы */
    map_put(map, "one", 1);
    map_put(map, "two", 2);
    map_put(map, "three", 3);
    map_put(map, "four", 4);
    map_put(map, "five", 5);

    check("Размер = 5", map_size(map) == 5);
    check("Словарь не пуст", map_is_empty(map) == false);

    /* Проверяем поиск */
    int val;
    check("one = 1", map_get(map, "one", &val) && val == 1);
    check("two = 2", map_get(map, "two", &val) && val == 2);
    check("three = 3", map_get(map, "three", &val) && val == 3);
    check("four = 4", map_get(map, "four", &val) && val == 4);
    check("five = 5", map_get(map, "five", &val) && val == 5);

    /* Проверяем отсутствующие ключи */
    check("six отсутствует", !map_contains(map, "six"));
    check("zero отсутствует", !map_contains(map, "zero"));

    /* map_get с NULL (только проверка) */
    check("map_get(one, NULL) работает", map_get(map, "one", NULL));

    map_print(map);
    map_destroy(map);
}

/*
 * Тест 2.3: Обновление значений
 */
static void test_map_update(void) {
    printf("\n--- Тест 2.3: Обновление значений ---\n");

    Map* map = map_create();

    map_put(map, "key", 10);
    check("key = 10", map_get(map, "key", NULL) && 1);

    map_put(map, "key", 20);
    check("key обновлён на 20", map_get(map, "key", NULL) && 1);

    int val;
    map_get(map, "key", &val);
    check("key = 20", val == 20);
    check("Размер не изменился (1)", map_size(map) == 1);

    /* Многократное обновление */
    map_put(map, "key", 30);
    map_put(map, "key", 40);
    map_put(map, "key", 50);
    map_get(map, "key", &val);
    check("key = 50", val == 50);
    check("Размер = 1", map_size(map) == 1);

    map_destroy(map);
}

/*
 * Тест 2.4: Удаление
 */
static void test_map_remove(void) {
    printf("\n--- Тест 2.4: Удаление ---\n");

    Map* map = map_create();

    map_put(map, "a", 1);
    map_put(map, "b", 2);
    map_put(map, "c", 3);
    map_put(map, "d", 4);
    map_put(map, "e", 5);

    check("Размер = 5", map_size(map) == 5);

    /* Удаляем */
    check("Удаление c", map_remove(map, "c"));
    check("Размер = 4", map_size(map) == 4);
    check("c отсутствует", !map_contains(map, "c"));

    check("Удаление a", map_remove(map, "a"));
    check("Размер = 3", map_size(map) == 3);
    check("a отсутствует", !map_contains(map, "a"));

    check("Удаление e", map_remove(map, "e"));
    check("Размер = 2", map_size(map) == 2);
    check("e отсутствует", !map_contains(map, "e"));

    /* Проверяем оставшиеся */
    check("b есть", map_contains(map, "b"));
    check("d есть", map_contains(map, "d"));

    /* Удаление несуществующего */
    check("Удаление x = false", !map_remove(map, "x"));
    check("Размер = 2", map_size(map) == 2);

    map_destroy(map);
}

/*
 * Тест 2.5: Очистка словаря
 */
static void test_map_clear(void) {
    printf("\n--- Тест 2.5: Очистка словаря ---\n");

    Map* map = map_create();

    /* Вставляем элементы */
    for (int i = 0; i < 10; i++) {
        char key[16];
        snprintf(key, sizeof(key), "key%d", i);
        map_put(map, key, i);
    }
    check("Размер = 10", map_size(map) == 10);

    /* Очищаем */
    map_clear(map);
    check("Размер после очистки = 0", map_size(map) == 0);
    check("Словарь пуст", map_is_empty(map) == true);
    check("key0 отсутствует", !map_contains(map, "key0"));
    check("key9 отсутствует", !map_contains(map, "key9"));

    /* После очистки можно добавлять */
    map_put(map, "new", 100);
    check("Размер = 1", map_size(map) == 1);
    int val;
    check("new = 100", map_get(map, "new", &val) && val == 100);

    map_destroy(map);
}


/* ============================================================================
 *                          ТЕСТ 3: ПРИМЕРЫ ПРИМЕНЕНИЯ
 * ============================================================================ */

 /*
  * Функция обратного вызова для печати элемента словаря
  */
static void print_entry(const char* key, int value) {
    printf("\"%s\":%d ", key, value);
}

/*
 * Вспомогательная структура для callback с пользовательскими данными
 */
typedef struct {
    int sum;
    int count;
} SumData;

/*
 * Функция для подсчёта суммы и количества элементов
 */
static void sum_callback(const char* key, int value, void* user_data) {
    SumData* data = (SumData*)user_data;
    data->sum += value;
    data->count++;
}

/*
 * Пример 1: Частотный словарь
 */
static void example_frequency_dictionary(void) {
    printf("\n--- Пример 1: Частотный словарь ---\n");

    const char* text[] = {
        "hello", "world", "hello", "hash", "table",
        "world", "hello", "red", "black", "tree",
        "hash", "tree", "hello", "world", "map",
        NULL
    };

    Map* freq = map_create();

    /* Подсчёт частоты слов */
    for (int i = 0; text[i]; i++) {
        int count = 0;
        map_get(freq, text[i], &count);
        map_put(freq, text[i], count + 1);
    }

    printf("Частотный словарь:\n");
    map_print(freq);

    /* Проверяем частоты */
    int val;
    check("hello = 4", map_get(freq, "hello", &val) && val == 4);
    check("world = 3", map_get(freq, "world", &val) && val == 3);
    check("hash = 2", map_get(freq, "hash", &val) && val == 2);
    check("tree = 2", map_get(freq, "tree", &val) && val == 2);
    check("red = 1", map_get(freq, "red", &val) && val == 1);
    check("black = 1", map_get(freq, "black", &val) && val == 1);
    check("map = 1", map_get(freq, "map", &val) && val == 1);

    map_destroy(freq);
}

/*
 * Пример 2: Телефонная книга
 */
static void example_phone_book(void) {
    printf("\n--- Пример 2: Телефонная книга ---\n");

    Map* phone_book = map_create();

    /* Добавляем контакты */
    map_put(phone_book, "Alice", 123456);
    map_put(phone_book, "Bob", 234567);
    map_put(phone_book, "Ivan", 345678);
    map_put(phone_book, "Diana", 456789);
    map_put(phone_book, "Eve", 567890);

    printf("Телефонная книга:\n");
    map_print(phone_book);

    /* Поиск номера */
    int phone;
    check("Alice: 123456", map_get(phone_book, "Alice", &phone) && phone == 123456);
    check("Bob: 234567", map_get(phone_book, "Bob", &phone) && phone == 234567);

    /* Обновляем номер */
    map_put(phone_book, "Alice", 111111);
    check("Alice обновлён: 111111", map_get(phone_book, "Alice", &phone) && phone == 111111);
    check("Размер не изменился (5)", map_size(phone_book) == 5);

    /* Удаляем контакт */
    map_remove(phone_book, "Eve");
    check("Eve удалён", !map_contains(phone_book, "Eve"));
    check("Размер = 4", map_size(phone_book) == 4);

    map_destroy(phone_book);
}

/*
 * Пример 3: Кэш результатов (LRU-like)
 */
static void example_cache(void) {
    printf("\n--- Пример 3: Кэш результатов ---\n");

    Map* cache = map_create();

    /* Симуляция кэширования вычислений */
    const char* queries[] = {
        "SELECT * FROM users",
        "SELECT * FROM products",
        "SELECT * FROM orders",
        "SELECT * FROM users",          /* Повторный запрос */
        "SELECT * FROM products",       /* Повторный запрос */
        "SELECT * FROM new_table",
        NULL
    };

    int cache_hits = 0;
    int cache_misses = 0;

    for (int i = 0; queries[i]; i++) {
        int result;
        if (map_get(cache, queries[i], &result)) {
            cache_hits++;
            printf("Кэш HIT: %s -> результат %d\n", queries[i], result);
        }
        else {
            cache_misses++;
            /* Имитация вычисления результата */
            int new_result = i * 10 + 5;
            map_put(cache, queries[i], new_result);
            printf("Кэш MISS: %s -> вычислен результат %d\n", queries[i], new_result);
        }
    }

    printf("Статистика кэша: хитов=%d, промахов=%d\n", cache_hits, cache_misses);
    printf("Размер кэша: %d\n", map_size(cache));

    check("Хитов кэша = 2", cache_hits == 2);
    check("Промахов кэша = 4", cache_misses == 4);
    check("Размер кэша = 4", map_size(cache) == 4);

    map_destroy(cache);
}

/*
 * Пример 4: Оценки студентов
 */
static void example_grades(void) {
    printf("\n--- Пример 4: Оценки студентов ---\n");

    Map* grades = map_create();

    /* Вводим оценки */
    map_put(grades, "Иванов", 85);
    map_put(grades, "Петров", 92);
    map_put(grades, "Сидоров", 78);
    map_put(grades, "Козлова", 95);
    map_put(grades, "Смирнов", 88);
    map_put(grades, "Иванов", 90);  /* Пересдача */

    printf("Оценки студентов:\n");
    map_print(grades);

    /* Проверяем успеваемость */
    int grade;
    check("Иванов: 90", map_get(grades, "Иванов", &grade) && grade == 90);
    check("Петров: 92", map_get(grades, "Петров", &grade) && grade == 92);
    check("Сидоров: 78", map_get(grades, "Сидоров", &grade) && grade == 78);

    /* Вычисляем средний балл через callback с пользовательскими данными */
    SumData data;              /* Объявляем переменную */
    data.sum = 0;              /* Инициализируем поле sum */
    data.count = 0;            /* Инициализируем поле count */
    map_for_each(grades, sum_callback, &data);

    printf("Средний балл: %.2f\n", (float)data.sum / data.count);

    check("Количество студентов = 5", map_size(grades) == 5);

    map_destroy(grades);
}


/* ============================================================================
 *                          ЗАПУСК ВСЕХ ТЕСТОВ
 * ============================================================================ */

static void run_all_tests(void) {
    /* Тесты красно-чёрного дерева */
    test_rb_create_destroy();
    test_rb_insert_search();
    test_rb_delete();
    test_rb_edge_cases();

    /* Тесты словаря */
    test_map_basic();
    test_map_put_get();
    test_map_update();
    test_map_remove();
    test_map_clear();

    /* Примеры применения */
    example_frequency_dictionary();
    example_phone_book();
    example_cache();
    example_grades();
}

/* ============================================================================
 *                          MAIN
 * ============================================================================ */

int main(void) {
    setlocale(LC_ALL, "Russian");

    printf("===============================================================\n");
    printf("     КРАСНО-ЧЁРНОЕ ДЕРЕВО И АССОЦИАТИВНЫЙ МАССИВ НА C\n");
    printf("===============================================================\n");

    run_all_tests();

    printf("\n===============================================================\n");
    printf("Итоги тестов:\n");
    printf("Пройдено: %d\n", passed);
    printf("Провалено: %d\n", failed);
    printf("Всего: %d\n", passed + failed);

    if (failed == 0) {
        printf("\nВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!\n");
    }
    else {
        printf("\nЕСТЬ ПРОВАЛЕННЫЕ ТЕСТЫ!\n");
    }

    printf("===============================================================\n");
    return 0;
}