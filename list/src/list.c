#include "../include/list.h"

#ifdef MOCK_MALLOC
#define malloc(size) mock_malloc(size)
void* mock_malloc(size_t size)
{
    (void) size;
    return NULL;
}
list_t global_list = {NULL, NULL, 0};
#endif

list_t *list_init(void)
{
    list_t *list;

    list = (list_t *) malloc(sizeof(list_t));
    if (list == NULL)
        return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

list_t *list_copy(list_t *l)
{
    list_t *list;
    list_node_t *node;

    if (l == NULL)
        return NULL;
    list = list_init();
    if (list == NULL)
        return NULL;
    node = l->head;
    while (node != NULL)
    {
        list_add_end(list, node->coeff);
        node = node->next;
    }
    return list;
}

list_error_t list_destroy(list_t *l)
{
    list_node_t *node, *tmp;

    if (l == NULL)
        return LIST_E_NULL_PTR;
    node = l->head;
    while (node != NULL)
    {
        tmp = node;
        node = node->next;
        free(tmp);
    }
    free(l);
    return LIST_E_SUCCESS;
}

list_error_t list_add_beginning(list_t *l, uint8_t coeff)
{
    list_node_t *node;

    if (l == NULL)
        return LIST_E_NULL_PTR;
    node = (list_node_t *) malloc(sizeof(list_node_t));
    if (node == NULL)
        return LIST_E_MEMORY;
    node->coeff = coeff;
    node->next = l->head;
    l->head = node;
    if (l->tail == NULL)
        l->tail = node;
    l->size++;
    return LIST_E_SUCCESS;
}

list_error_t list_add_end(list_t *l, uint8_t coeff)
{
    list_node_t *node;

    if (l == NULL)
        return LIST_E_NULL_PTR;
    node = (list_node_t *) malloc(sizeof(list_node_t));
    if (node == NULL)
        return LIST_E_MEMORY;
    node->coeff = coeff;
    node->next = NULL;
    if (l->tail == NULL)
    {
        l->head = node;
        l->tail = node;
    }
    else
    {
        l->tail->next = node;
        l->tail = node;
    }
    l->size++;
    return LIST_E_SUCCESS;
}

list_error_t list_add_at(list_t *l, uint8_t coeff, size_t pos)
{
    list_node_t *node, *tmp;
    size_t i;

    if (l == NULL)
        return LIST_E_NULL_PTR;
    if (pos > l->size)
        return LIST_E_INVALID_ARGUMENT;
    if (pos == 0)
        return list_add_beginning(l, coeff);
    if (pos == l->size)
        return list_add_end(l, coeff);
    node = (list_node_t *) malloc(sizeof(list_node_t));
    if (node == NULL)
        return LIST_E_MEMORY;
    node->coeff = coeff;
    tmp = l->head;
    for (i = 0; i < pos - 1; i++)
        tmp = tmp->next;
    node->next = tmp->next;
    tmp->next = node;
    l->size++;
    return LIST_E_SUCCESS;
}

list_error_t list_add_after(list_t *l, uint8_t coeff, list_node_t *node)
{
    list_node_t *new_node;

    if (l == NULL)
        return LIST_E_NULL_PTR;
    if (node == NULL)
        return LIST_E_INVALID_ARGUMENT;
    if (node == l->tail)
        return list_add_end(l, coeff);
    new_node = (list_node_t *) malloc(sizeof(list_node_t));
    if (new_node == NULL)
        return LIST_E_MEMORY;
    new_node->coeff = coeff;
    new_node->next = node->next;
    node->next = new_node;
    l->size++;
    return LIST_E_SUCCESS;
}

list_error_t list_remove_coeff(list_t *l, uint8_t coeff)
{
    list_node_t *tmp;

    if (l == NULL)
        return LIST_E_NULL_PTR;
    if (l->head == NULL)
        return LIST_E_EMPTY;
    if (l->head->coeff == coeff)
    {
        list_remove_head(l);
        return LIST_E_SUCCESS;
    }
    tmp = l->head;
    while (tmp->next != NULL)
    {
        if (tmp->next->coeff == coeff)
        {
            list_remove_node(l, tmp->next);
            return LIST_E_SUCCESS;
        }
        tmp = tmp->next;
    }
    return LIST_E_NOT_FOUND;
}

list_error_t list_remove_node(list_t *l, list_node_t *node)
{
    list_node_t *tmp;

    if (l == NULL)
        return LIST_E_NULL_PTR;
    if (l->head == NULL)
        return LIST_E_EMPTY;
    if (node == NULL)
        return LIST_E_INVALID_ARGUMENT;
    if (node == l->head)
        return list_remove_head(l);
    else if (node == l->tail)
        return list_remove_tail(l);
    else
    {
        tmp = l->head;
        while (tmp != NULL)
        {
            if (tmp->next == node)
            {
                tmp->next = node->next;
                free(node);
                l->size--;
                return LIST_E_SUCCESS;
            }
            tmp = tmp->next;
        }
    }
    return LIST_E_NOT_FOUND;
}

list_error_t list_remove_head(list_t *l)
{
    list_node_t *tmp;

    if (l == NULL)
        return LIST_E_NULL_PTR;
    if (l->head == NULL)
        return LIST_E_EMPTY;
    tmp = l->head;
    l->head = l->head->next;
    free(tmp);
    l->size--;
    return LIST_E_SUCCESS;
}

list_error_t list_remove_tail(list_t *l)
{
    list_node_t *tmp;

    if (l == NULL)
        return LIST_E_NULL_PTR;
    if (l->tail == NULL)
        return LIST_E_EMPTY;
    tmp = l->head;
    while (tmp != NULL)
    {
        if (tmp->next == l->tail)
        {
            free(l->tail);
            l->tail = tmp;
            l->tail->next = NULL;
            l->size--;
            return LIST_E_SUCCESS;
        }
        tmp = tmp->next;
    }
    return LIST_E_NOT_FOUND;
}

list_error_t list_get_pos(list_t* l, list_node_t *node)
{
    list_node_t *tmp;
    size_t i;

    if (l == NULL)
        return LIST_E_NULL_PTR;
    if (node == NULL)
        return LIST_E_INVALID_ARGUMENT;
    tmp = l->head;
    for (i = 0; i < l->size; i++)
    {
        if (tmp == node)
            return i;
        tmp = tmp->next;
    }
    return LIST_E_NOT_FOUND;
}

list_node_t *list_get_at_pos(list_t *l, size_t pos)
{
    list_node_t *tmp;
    size_t i;

    if (l == NULL)
        return NULL;
    if (pos >= l->size)
        return NULL;
    tmp = l->head;
    for (i = 0; i < pos; i++)
        tmp = tmp->next;
    return tmp;
}

list_error_t list_print(FILE *fd, list_t *l)
{
    list_node_t *node;

    if (l == NULL)
        return LIST_E_NULL_PTR;
    if (fd == NULL)
        return LIST_E_FD_IS_NULL;
    node = l->head;
    while (node != NULL)
    {
        fprintf(fd, "%d ", node->coeff);
        node = node->next;
    }
    fprintf(fd, "\n");
    return LIST_E_SUCCESS;
}

list_error_t list_assert(list_t *l, uint8_t *coeffs, size_t size)
{
    list_node_t *node;
    size_t i;

    if (l == NULL)
        return LIST_E_NULL_PTR;
    if (coeffs == NULL)
        return LIST_E_INVALID_ARGUMENT;
    if (l->size != size)
        return LIST_E_INVALID_ARGUMENT;
    node = l->head;
    for (i = 0; i < size; i++)
    {
        if (node->coeff != coeffs[i])
            return LIST_E_INVALID_ARGUMENT;
        node = node->next;
    }
    return LIST_E_SUCCESS;
}

list_t *list_create_from_array(uint8_t *coeffs, size_t size)
{
    list_t *l;
    size_t i;

    l = list_init();
    if (l == NULL)
        return NULL;
    if (coeffs == NULL)
    {
        list_destroy(l);
        return NULL;
    }
    for (i = 0; i < size; i++)
    {
        if (list_add_end(l, coeffs[i]) != LIST_E_SUCCESS)
        {
            list_destroy(l);
            return NULL;
        }
    }
    return l;
}