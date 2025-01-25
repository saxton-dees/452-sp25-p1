#include "lab.h"
#include <stdio.h>
#include <stdlib.h>

// Function to initialize a new doubly linked list with a sentinel node.
//  destroy_data: Function pointer to destroy the data stored in a node.
//  compare_to: Function pointer to compare data in two nodes.
//  Returns: Pointer to the newly created list, or NULL on failure.
list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
    // Allocate memory for the list structure.
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if (!list) return NULL;

    list->destroy_data = destroy_data;
    list->compare_to = compare_to;
    list->size = 0;

    // Allocate and initialize the sentinel node.
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

// Function to destroy a doubly linked list.
//  list: Pointer to a pointer to the list to be destroyed.
void list_destroy(list_t **list) {
    if (!*list) return;

    node_t *current = (*list)->head->next;
    while (current != (*list)->head) {
        node_t *next = current->next;
        if ((*list)->destroy_data && current->data) {
            (*list)->destroy_data(current->data); // Free data if a destroy function is provided.
        }
        free(current); // Free the node itself.
        current = next;
    }

    free((*list)->head); // Free the sentinel node.
    free(*list);         // Free the list structure.
    *list = NULL;        // Set the original pointer to NULL.
}

// Function to add data to the front of a doubly linked list.
//  list: Pointer to the list.
//  data: Pointer to the data to be added.
//  Returns: Pointer to the list, or NULL on failure.
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

// Function to remove a node at a specific index from a doubly linked list.
//  list: Pointer to the list.
//  index: Index of the node to be removed.
//  Returns: Pointer to the data stored in the removed node, or NULL on failure.
void *list_remove_index(list_t *list, size_t index) {
    if (!list || index >= list->size) return NULL;

    node_t *current;
    // Optimize traversal based on index position.
    if (index < list->size / 2) {
        current = list->head->next;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
    } else {
        current = list->head->prev;
        for (size_t i = list->size - 1; i > index; i--) {
            current = current->prev;
        }
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;

    void *data = current->data;
    free(current); // Free the node itself.
    list->size--;

    return data;
}

// Function to search for the index of the first occurrence of data in the list.
//  list: Pointer to the list.
//  data: Pointer to the data to search for.
//  Returns: Index of the first occurrence of data, or -1 if not found.
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
