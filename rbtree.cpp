#define _CRT_SECURE_NO_WARNINGS

#include "rbtree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Создание нового узла
 * Выделяет память, копирует ключ, инициализирует поля
 */
RBNode* rb_create_node(const char* key, int value) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    if (!node) return NULL;

    node->key = (char*)malloc(strlen(key) + 1);
    if (!node->key) {
        free(node);
        return NULL;
    }
    strcpy(node->key, key);

    node->value = value;
    node->color = RED;          /* Новый узел всегда красный */
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

/*
 * Создание пустого красно-чёрного дерева
 */
RBTree* rb_create(void) {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    if (!tree) return NULL;

    tree->root = NULL;
    tree->size = 0;

    return tree;
}

/*
 * Рекурсивное удаление узла и всех его потомков
 */
void rb_destroy_node(RBNode* node) {
    if (!node) return;

    rb_destroy_node(node->left);    /* Удаляем левое поддерево */
    rb_destroy_node(node->right);   /* Удаляем правое поддерево */

    free(node->key);                /* Освобождаем ключ */
    free(node);                     /* Освобождаем узел */
}

/*
 * Удаление всего дерева
 */
void rb_destroy(RBTree* tree) {
    if (!tree) return;

    rb_destroy_node(tree->root);    /* Удаляем все узлы */
    free(tree);                     /* Освобождаем структуру дерева */
}

/*
 * Левый поворот вокруг узла x
 * Преобразует: x -> y (y становится родителем x)
 */
void rb_rotate_left(RBTree* tree, RBNode* x) {
    RBNode* y = x->right;           /* y - правый потомок x */
    x->right = y->left;             /* Левое поддерево y становится правым поддеревом x */

    if (y->left != NULL) {
        y->left->parent = x;        /* Обновляем родителя для левого поддерева y */
    }

    y->parent = x->parent;          /* y поднимается на место x */

    if (x->parent == NULL) {
        tree->root = y;             /* Если x был корнем, y становится корнем */
    }
    else if (x == x->parent->left) {
        x->parent->left = y;        /* Если x был левым потомком, y становится левым */
    }
    else {
        x->parent->right = y;       /* Если x был правым потомком, y становится правым */
    }

    y->left = x;                    /* x становится левым потомком y */
    x->parent = y;                  /* Обновляем родителя x */
}

/*
 * Правый поворот вокруг узла y
 * Преобразует: y <- x (x становится родителем y)
 */
void rb_rotate_right(RBTree* tree, RBNode* y) {
    RBNode* x = y->left;            /* x - левый потомок y */
    y->left = x->right;             /* Правое поддерево x становится левым поддеревом y */

    if (x->right != NULL) {
        x->right->parent = y;       /* Обновляем родителя для правого поддерева x */
    }

    x->parent = y->parent;          /* x поднимается на место y */

    if (y->parent == NULL) {
        tree->root = x;             /* Если y был корнем, x становится корнем */
    }
    else if (y == y->parent->left) {
        y->parent->left = x;        /* Если y был левым потомком, x становится левым */
    }
    else {
        y->parent->right = x;       /* Если y был правым потомком, x становится правым */
    }

    x->right = y;                   /* y становится правым потомком x */
    y->parent = x;                  /* Обновляем родителя y */
}

/*
 * Восстановление свойств красно-чёрного дерева после вставки
 * Исправляет нарушения, вызванные вставкой красного узла
 */
void rb_insert_fixup(RBTree* tree, RBNode* node) {
    /* Пока родитель узла красный (нарушение свойства 4) */
    while (node->parent != NULL && node->parent->color == RED) {
        RBNode* parent = node->parent;
        RBNode* grandparent = parent->parent;

        if (parent == grandparent->left) {
            /* Родитель — левый потомок дедушки */
            RBNode* uncle = grandparent->right;

            if (uncle != NULL && uncle->color == RED) {
                /* Случай 1: Дядя красный — перекрашиваем */
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;     /* Поднимаемся к дедушке */
            }
            else {
                /* Случай 2: Дядя чёрный */
                if (node == parent->right) {
                    /* Если узел — правый потомок, делаем левый поворот */
                    node = parent;
                    rb_rotate_left(tree, node);
                    parent = node->parent;
                }

                /* Случай 3: Узел — левый потомок */
                parent->color = BLACK;
                grandparent->color = RED;
                rb_rotate_right(tree, grandparent);
            }
        }
        else {
            /* Родитель — правый потомок дедушки (симметричный случай) */
            RBNode* uncle = grandparent->left;

            if (uncle != NULL && uncle->color == RED) {
                /* Случай 1: Дядя красный — перекрашиваем */
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }
            else {
                /* Случай 2: Дядя чёрный */
                if (node == parent->left) {
                    /* Если узел — левый потомок, делаем правый поворот */
                    node = parent;
                    rb_rotate_right(tree, node);
                    parent = node->parent;
                }

                /* Случай 3: Узел — правый потомок */
                parent->color = BLACK;
                grandparent->color = RED;
                rb_rotate_left(tree, grandparent);
            }
        }
    }

    /* Корень всегда чёрный (свойство 2) */
    tree->root->color = BLACK;
}

/*
 * Вставка ключа и значения в красно-чёрное дерево
 * Если ключ уже существует, обновляет значение
 */
void rb_insert(RBTree* tree, const char* key, int value) {
    /* Создаём новый узел */
    RBNode* new_node = rb_create_node(key, value);
    if (!new_node) return;

    RBNode* current = tree->root;
    RBNode* parent = NULL;

    /* Ищем место для вставки */
    while (current != NULL) {
        parent = current;
        int cmp = strcmp(key, current->key);

        if (cmp == 0) {
            /* Ключ уже существует — обновляем значение */
            current->value = value;
            free(new_node->key);
            free(new_node);
            return;
        }
        else if (cmp < 0) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    /* Вставляем новый узел */
    new_node->parent = parent;

    if (parent == NULL) {
        /* Дерево было пустым — новый узел становится корнем */
        tree->root = new_node;
    }
    else if (strcmp(key, parent->key) < 0) {
        parent->left = new_node;
    }
    else {
        parent->right = new_node;
    }

    tree->size++;

    /* Восстанавливаем свойства красно-чёрного дерева */
    rb_insert_fixup(tree, new_node);
}

/*
 * Поиск узла по ключу
 * Возвращает указатель на узел или NULL
 */
RBNode* rb_search(RBTree* tree, const char* key) {
    RBNode* current = tree->root;

    while (current != NULL) {
        int cmp = strcmp(key, current->key);

        if (cmp == 0) {
            return current;
        }
        else if (cmp < 0) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    return NULL;
}

/*
 * Проверка наличия ключа в дереве
 */
bool rb_contains(RBTree* tree, const char* key) {
    return rb_search(tree, key) != NULL;
}

/*
 * Получение значения по ключу
 * Возвращает true если ключ найден, false если нет
 * Значение записывается в out_value
 */
bool rb_get(RBTree* tree, const char* key, int* out_value) {
    RBNode* node = rb_search(tree, key);
    if (!node) return false;

    if (out_value) *out_value = node->value;
    return true;
}

/*
 * Поиск минимального узла в поддереве
 */
RBNode* rb_minimum(RBNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

/*
 * Замена одного поддерева другим (transplant)
 * Заменяет узел u на узел v в дереве
 */
void rb_transplant(RBTree* tree, RBNode* u, RBNode* v) {
    if (u->parent == NULL) {
        tree->root = v;              /* u был корнем */
    }
    else if (u == u->parent->left) {
        u->parent->left = v;         /* u был левым потомком */
    }
    else {
        u->parent->right = v;        /* u был правым потомком */
    }

    if (v != NULL) {
        v->parent = u->parent;       /* Обновляем родителя для v */
    }
}

/*
 * Восстановление свойств красно-чёрного дерева после удаления
 * Исправляет двойную чёрную высоту
 */
void rb_delete_fixup(RBTree* tree, RBNode* node) {
    /* Если node == NULL, то это особый случай - удаление листа */
    if (node == NULL) {
        /* Если дерево не пустое, корень должен быть чёрным */
        if (tree->root != NULL) {
            tree->root->color = BLACK;
        }
        return;
    }

    while (node != tree->root && node->color == BLACK) {
        /* Проверяем, что parent существует */
        if (node->parent == NULL) {
            break;  /* Защита от неожиданного NULL */
        }

        if (node == node->parent->left) {
            /* Узел — левый потомок */
            RBNode* sibling = node->parent->right;

            /* Проверяем, что sibling существует */
            if (sibling == NULL) {
                /* Если брата нет, просто поднимаемся вверх */
                node = node->parent;
                continue;
            }

            if (sibling->color == RED) {
                /* Случай 1: Брат красный */
                sibling->color = BLACK;
                node->parent->color = RED;
                rb_rotate_left(tree, node->parent);
                sibling = node->parent->right;

                /* Проверяем, что sibling существует после поворота */
                if (sibling == NULL) {
                    node = node->parent;
                    continue;
                }
            }

            if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                (sibling->right == NULL || sibling->right->color == BLACK)) {
                /* Случай 2: Оба племянника чёрные */
                sibling->color = RED;
                node = node->parent;
            }
            else {
                if (sibling->right == NULL || sibling->right->color == BLACK) {
                    /* Случай 3: Правый племянник чёрный */
                    if (sibling->left != NULL) {
                        sibling->left->color = BLACK;
                    }
                    sibling->color = RED;
                    rb_rotate_right(tree, sibling);
                    sibling = node->parent->right;

                    /* Проверяем, что sibling существует после поворота */
                    if (sibling == NULL) {
                        node = node->parent;
                        continue;
                    }
                }

                /* Случай 4: Правый племянник красный */
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                if (sibling->right != NULL) {
                    sibling->right->color = BLACK;
                }
                rb_rotate_left(tree, node->parent);
                node = tree->root;
            }
        }
        else {
            /* Узел — правый потомок (симметричный случай) */
            RBNode* sibling = node->parent->left;

            /* Проверяем, что sibling существует */
            if (sibling == NULL) {
                node = node->parent;
                continue;
            }

            if (sibling->color == RED) {
                /* Случай 1: Брат красный */
                sibling->color = BLACK;
                node->parent->color = RED;
                rb_rotate_right(tree, node->parent);
                sibling = node->parent->left;

                if (sibling == NULL) {
                    node = node->parent;
                    continue;
                }
            }

            if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                (sibling->right == NULL || sibling->right->color == BLACK)) {
                /* Случай 2: Оба племянника чёрные */
                sibling->color = RED;
                node = node->parent;
            }
            else {
                if (sibling->left == NULL || sibling->left->color == BLACK) {
                    /* Случай 3: Левый племянник чёрный */
                    if (sibling->right != NULL) {
                        sibling->right->color = BLACK;
                    }
                    sibling->color = RED;
                    rb_rotate_left(tree, sibling);
                    sibling = node->parent->left;

                    if (sibling == NULL) {
                        node = node->parent;
                        continue;
                    }
                }

                /* Случай 4: Левый племянник красный */
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                if (sibling->left != NULL) {
                    sibling->left->color = BLACK;
                }
                rb_rotate_right(tree, node->parent);
                node = tree->root;
            }
        }
    }

    if (node != NULL) {
        node->color = BLACK;
    }
}

/*
 * Удаление ключа из красно-чёрного дерева
 * Возвращает true если ключ найден и удалён, иначе false
 */
bool rb_delete(RBTree* tree, const char* key) {
    RBNode* node = rb_search(tree, key);
    if (!node) return false;

    RBNode* y = node;
    RBNode* x = NULL;
    Color y_original_color = y->color;

    if (node->left == NULL) {
        /* У узла нет левого потомка */
        x = node->right;
        rb_transplant(tree, node, node->right);
    }
    else if (node->right == NULL) {
        /* У узла нет правого потомка */
        x = node->left;
        rb_transplant(tree, node, node->left);
    }
    else {
        /* У узла есть оба потомка — ищем преемника */
        y = rb_minimum(node->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == node) {
            if (x != NULL) {
                x->parent = y;
            }
        }
        else {
            rb_transplant(tree, y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }

        rb_transplant(tree, node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }

    free(node->key);
    free(node);
    tree->size--;

    /* Если удалённый узел был чёрным, нужно восстановить баланс */
    if (y_original_color == BLACK) {
        /* Если x == NULL, это особый случай */
        if (x == NULL) {
            /* Если удалили чёрный лист, нужно проверить корень */
            if (tree->root != NULL) {
                tree->root->color = BLACK;
            }
        }
        else {
            rb_delete_fixup(tree, x);
        }
    }

    return true;
}

/*
 * Получение количества элементов в дереве
 */
int rb_size(RBTree* tree) {
    return tree->size;
}

/*
 * Рекурсивный обход дерева с передачей пользовательских данных
 */
static void rb_inorder_recursive(RBNode* node, void (*callback)(const char* key, int value, void* user_data), void* user_data) {
    if (!node) return;

    rb_inorder_recursive(node->left, callback, user_data);
    callback(node->key, node->value, user_data);
    rb_inorder_recursive(node->right, callback, user_data);
}

/*
 * Обход дерева с передачей пользовательских данных
 */
void rb_inorder(RBTree* tree, void (*callback)(const char* key, int value, void* user_data), void* user_data) {
    rb_inorder_recursive(tree->root, callback, user_data);
}

/*
 * Рекурсивная печать дерева (для отладки)
 */
void rb_print_node(RBNode* node, int level, const char* side) {
    if (!node) return;

    /* Выводим отступы для визуализации структуры */
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%s%s(%s): %d\n", side, node->key, node->color == RED ? "R" : "B", node->value);

    rb_print_node(node->left, level + 1, "L-");
    rb_print_node(node->right, level + 1, "R-");
}

/*
 * Печать всего дерева
 */
void rb_print(RBTree* tree) {
    if (!tree->root) {
        printf("Дерево пустое\n");
        return;
    }

    printf("Красно-чёрное дерево (size=%d):\n", tree->size);
    rb_print_node(tree->root, 0, "");
}
