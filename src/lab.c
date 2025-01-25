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

    // Allocate and initialize the sentinel node
    list->head = (node_t *)malloc(sizeof(node_t));
    if (!list->head) {
        free(list);
        return NULL;
    }
    list->head->data = NULL;
    list->head->next = list->head;
    list->head->prev = list->head;

    return list;
}

void list_destroy(list_t **list) {
    if (!list || !*list) return;

    node_t *current = (*list)->head->next;
    while (current != (*list)->head) {
        node_t *next = current->next;
        if ((*list)->destroy_data && current->data) {
            (*list)->destroy_data(current->data); // Frees data if allocated
        }
        free(current); // Free the node itself
        current = next;
    }

    free((*list)->head); // Free the sentinel node
    free(*list);
    *list = NULL;
}

// Function to add data to the front of the list
list_t *list_add(list_t *list, void *data) {
    if (!list) return NULL;

    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) return NULL;

    new_node->data = data;
    new_node->next = list->head->next;
    new_node->prev = list->head;

    list->head->next->prev = new_node;
    list->head->next = new_node;

    list->size++;

    return list;
}

void *list_remove_index(list_t *list, size_t index) {
    if (!list || index >= list->size) return NULL;

    node_t *current = list->head->next;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;

    void *data = current->data;
    free(current); // Free the node itself
    list->size--;

    return data;
}

// Function to search for any occurrence of data from the list
int list_indexof(list_t *list, void *data) {
    if (!list) return -1;

    node_t *current = list->head->next;
    int index = 0;
    while (current != list->head) {
        if (list->compare_to(current->data, data) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}
