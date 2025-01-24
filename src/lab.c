#include "lab.h"
#include <stdio.h>
#include <stdlib.h>

// Function to initialize a new list
list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if (!list) return NULL;

    list->destroy_data = destroy_data;
    list->compare_to = compare_to;
    list->size = 0;
    list->head = NULL;

    return list;
}

// Function to destroy the list and all associated data
void list_destroy(list_t **list) {
    if (!list || !*list) return;

    node_t *current = (*list)->head;
    while (current) {
        node_t *next = current->next;
        if ((*list)->destroy_data) {
            (*list)->destroy_data(current->data);
        }
        free(current);
        current = next;
    }

    free(*list);
    *list = NULL;
}

// Function to add data to the front of the list
list_t *list_add(list_t *list, void *data) {
    if (!list) return NULL;

    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) return NULL;

    new_node->data = data;
    new_node->next = list->head;
    new_node->prev = NULL;

    if (list->head) {
        list->head->prev = new_node;
    }

    list->head = new_node;
    list->size++;

    return list;
}

// Function to remove data at the specified index
void *list_remove_index(list_t *list, size_t index) {
    if (!list || index >= list->size) return NULL;

    node_t *current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    if (current->prev) {
        current->prev->next = current->next;
    } else {
        list->head = current->next;
    }

    if (current->next) {
        current->next->prev = current->prev;
    }

    void *data = current->data;
    free(current);
    list->size--;

    return data;
}

// Function to search for any occurrence of data from the list
int list_indexof(list_t *list, void *data) {
    if (!list) return -1;

    node_t *current = list->head;
    int index = 0;
    while (current) {
        if (list->compare_to(current->data, data) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}
