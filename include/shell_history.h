#ifndef _SHELL_HISTORY_H
#define _SHELL_HISTORY_H 1

#include "tty.h"
#include "memory.h"
#include "string.h"

#define BUFFER_SIZE 1024

typedef struct node
{
    char buffer[BUFFER_SIZE];
    struct node *next;
} node_t;

node_t *create_new_node(char buffer[BUFFER_SIZE]);
void print_history(node_t *head);
void *insert_at_head(node_t **head, node_t *node_to_insert);
int serialize_history(node_t *head, uint8_t *buffer, uint32_t max_size);

#endif
