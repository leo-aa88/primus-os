#include "../include/shell_history.h"

node_t *create_new_node(char buffer[BUFFER_SIZE])
{
    node_t *result = (node_t *)kmalloc(sizeof(node_t));
    strcpy(result->buffer, buffer);
    result->next = NULL;
    return result;
}

void print_history(node_t *head)
{
    node_t *temporary = head;

    printk("\n");
    while (temporary != NULL)
    {
        printk("%s\n", temporary->buffer);
        temporary = temporary->next;
    }
}

void *insert_at_head(node_t **head, node_t *node_to_insert)
{
    node_to_insert->next = *head;
    *head = node_to_insert;
}

int serialize_history(node_t *head, uint8_t *buffer, uint32_t max_size)
{
    uint32_t offset = 0;
    node_t *cur = head;
    while (cur != NULL)
    {
        uint32_t len = strlen(cur->buffer);
        if (offset + len + 1 >= max_size)
            break;
        strcpy((char *)&buffer[offset], cur->buffer);
        offset += len;
        buffer[offset++] = '\n';
        cur = cur->next;
    }
    buffer[offset] = '\0';
    return offset;
}
