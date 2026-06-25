#ifndef RBTREE_H
#define RBTREE_H

#include <stdbool.h>
#include <stdint.h>

/* Цвета узлов красно-чёрного дерева */
typedef enum { RED, BLACK } Color;

/* Узел красно-чёрного дерева */
typedef struct RBNode {
    char* key;                  /* Ключ (строка) */
    int value;                  /* Значение (число) */
    Color color;                /* Цвет узла: RED или BLACK */
    struct RBNode* left;        /* Левый потомок */
    struct RBNode* right;       /* Правый потомок */
    struct RBNode* parent;      /* Родительский узел */
} RBNode;

/* Красно-чёрное дерево */
typedef struct {
    RBNode* root;               /* Корень дерева */
    int size;                   /* Количество элементов в дереве */
} RBTree;

/*
 * Функции для работы с красно-чёрным деревом
 */

 /* Создание и удаление дерева */
RBTree* rb_create(void);
void rb_destroy(RBTree* tree);
void rb_destroy_node(RBNode* node);

/* Вставка и обновление */
void rb_insert(RBTree* tree, const char* key, int value);
void rb_insert_fixup(RBTree* tree, RBNode* node);

/* Поиск */
RBNode* rb_search(RBTree* tree, const char* key);
bool rb_contains(RBTree* tree, const char* key);
bool rb_get(RBTree* tree, const char* key, int* out_value);

/* Удаление */
bool rb_delete(RBTree* tree, const char* key);
void rb_delete_fixup(RBTree* tree, RBNode* node);
void rb_transplant(RBTree* tree, RBNode* u, RBNode* v);
RBNode* rb_minimum(RBNode* node);

/* Вспомогательные функции */
RBNode* rb_create_node(const char* key, int value);
void rb_rotate_left(RBTree* tree, RBNode* node);
void rb_rotate_right(RBTree* tree, RBNode* node);
void rb_print(RBTree* tree);
void rb_print_node(RBNode* node, int level, const char* side);

/* Получение размера */
int rb_size(RBTree* tree);

/* Обход дерева с передачей пользовательских данных */
void rb_inorder(RBTree* tree, void (*callback)(const char* key, int value, void* user_data), void* user_data);

#endif /* RBTREE_H */