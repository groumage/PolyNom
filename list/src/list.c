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

static void list_error(list_error_t err, const char *file, const char *fct, const int line, const char *error)
{
    switch (err)
    {
        case LIST_E_MEMORY:
            fprintf(stderr, "Error in [%s, %s] line %d: memory error.\n", file, fct, line);
            break;
        case LIST_E_LIST_IS_NULL:
            fprintf(stderr, "Error in [%s, %s] line %d: list is NULL.\n", file, fct, line);
            break;
        case LIST_E_LIST_MANIPULATION:
            fprintf(stderr, "Error in [%s, %s] line %d: %s.\n", file, fct, line, error);
            break;
        case LIST_E_FILE_DESCRIPTOR_IS_NULL:
            fprintf(stderr, "Error in [%s, %s] line %d: file descriptor is NULL.\n", file, fct, line);
            break;
        default:
            break;
    }
}

static void list_error_no_custom_msg(list_error_t err, const char *file, const char *fct, const int line)
{
    list_error(err, file, fct, line, NULL);
}

list_t *list_init(void)
{
    list_t *list = (list_t *) malloc(sizeof(list_t));
    if (list == NULL)
    {
        list_error_no_custom_msg(LIST_E_MEMORY, __FILE__, __func__, __LINE__);
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

list_t *list_copy(list_t *l)
{
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return NULL;
    }
    list_t *list = list_init();
    if (list == NULL)
    {
        list_error_no_custom_msg(LIST_E_MEMORY, __FILE__, __func__, __LINE__);
        return NULL;
    }
    list_node_t *node = l->head;
    while (node != NULL)
    {
        if (list_add_end(list, node->coeff) != LIST_E_SUCCESS)
        {
            if (list_destroy(list) != LIST_E_SUCCESS)
            {
                list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "list_destroy() failed");
                return NULL;
            }
            list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "list_add_end() failed");
            return NULL;
        }
        node = node->next;
    }
    return list;
}

list_error_t list_destroy(list_t *l)
{
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_LIST_IS_NULL;
    }
    list_node_t *node = l->head;
    while (node != NULL)
    {
        list_node_t *tmp = node;
        node = node->next;
        free(tmp);
    }
    free(l);
    return LIST_E_SUCCESS;
}

list_error_t list_add_beginning(list_t *l, uint8_t coeff)
{
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_LIST_IS_NULL;
    }
    list_node_t *node = (list_node_t *) malloc(sizeof(list_node_t));
    if (node == NULL)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "node is NULL");
        return LIST_E_LIST_MANIPULATION;
    }
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
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_LIST_IS_NULL;
    }
    list_node_t *node = (list_node_t *) malloc(sizeof(list_node_t));
    if (node == NULL)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "node is NULL");
        return LIST_E_LIST_MANIPULATION;
    }
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
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_LIST_IS_NULL;
    }
    if (pos > l->size)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "requested pos is greater than the size of the list");
        return LIST_E_LIST_MANIPULATION;
    }
    if (pos == 0)
        return list_add_beginning(l, coeff);
    if (pos == l->size)
        return list_add_end(l, coeff);
    list_node_t *node = (list_node_t *) malloc(sizeof(list_node_t));
    if (node == NULL)
    {
        list_error(LIST_E_MEMORY, __FILE__, __func__, __LINE__, NULL);
        return LIST_E_MEMORY;
    }
    node->coeff = coeff;
    list_node_t *tmp = l->head;
    for (size_t i = 0; i < pos - 1; i++)
        tmp = tmp->next;
    node->next = tmp->next;
    tmp->next = node;
    l->size++;
    return LIST_E_SUCCESS;
}

list_error_t list_add_after(list_t *l, uint8_t coeff, list_node_t *node)
{
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_LIST_IS_NULL;
    }
    if (node == NULL)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "node is NULL");
        return LIST_E_LIST_MANIPULATION;
    }
    if (node == l->tail)
        return list_add_end(l, coeff);
    list_node_t *new_node = (list_node_t *) malloc(sizeof(list_node_t));
    if (new_node == NULL)
    {
        list_error_no_custom_msg(LIST_E_MEMORY, __FILE__, __func__, __LINE__);
        return LIST_E_MEMORY;
    }
    new_node->coeff = coeff;
    new_node->next = node->next;
    node->next = new_node;
    l->size++;
    return LIST_E_SUCCESS;
}

list_error_t list_remove_coeff(list_t *l, uint8_t coeff)
{
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_LIST_IS_NULL;
    }
    if (l->head == NULL)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "head of list is NULL");
        return LIST_E_LIST_MANIPULATION;
    }
    if (l->head->coeff == coeff)
        return list_remove_head(l);
    list_node_t *tmp = l->head;
    while (tmp->next != NULL)
    {
        if (tmp->next->coeff == coeff)
            return list_remove_node(l, tmp->next);
        tmp = tmp->next;
    }
    list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "coefficient not found");
    return LIST_E_LIST_MANIPULATION;
}

list_error_t list_remove_node(list_t *l, list_node_t *node)
{
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_LIST_IS_NULL;
    }
    if (l->head == NULL)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "head of list is NULL");
        return LIST_E_LIST_MANIPULATION;
    }
    if (node == NULL)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "node is NULL");
        return LIST_E_LIST_MANIPULATION;
    }
    if (node == l->head)
        return list_remove_head(l);
    else if (node == l->tail)
        return list_remove_tail(l);
    else
    {
        list_node_t *tmp = l->head;
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
    list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "node not found");
    return LIST_E_LIST_MANIPULATION;
}

list_error_t list_remove_head(list_t *l)
{
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_LIST_IS_NULL;
    }
    if (l->head == NULL)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "head of list is NULL");
        return LIST_E_LIST_MANIPULATION;
    }
    list_node_t *tmp = l->head;
    l->head = l->head->next;
    free(tmp);
    l->size--;
    return LIST_E_SUCCESS;
}

list_error_t list_remove_tail(list_t *l)
{
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_LIST_IS_NULL;
    }
    if (l->tail == NULL)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "tail of list is NULL");
        return LIST_E_LIST_MANIPULATION;
    }
    list_node_t *tmp = l->head;
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
    list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "tail not found");
    return LIST_E_LIST_MANIPULATION;
}

list_error_t list_get_pos(list_t* l, list_node_t *node)
{
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_LIST_IS_NULL;
    }
    if (node == NULL)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "node is NULL");
        return LIST_E_LIST_MANIPULATION;
    }
    list_node_t *tmp = l->head;
    for (size_t i = 0; i < l->size; i++)
    {
        if (tmp == node)
            return i;
        tmp = tmp->next;
    }
    list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "node not found");
    return LIST_E_LIST_MANIPULATION;
}

list_node_t *list_get_at_pos(list_t *l, size_t pos)
{
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return NULL;
    }
    if (pos >= l->size)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "requested pos is greater than the size of the list");
        return NULL;
    }
    list_node_t *tmp = l->head;
    for (size_t i = 0; i < pos; i++)
        tmp = tmp->next;
    return tmp;
}

list_error_t list_print(FILE *fd, list_t *l)
{
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_LIST_IS_NULL;
    }
    if (fd == NULL)
    {
        list_error_no_custom_msg(LIST_E_FILE_DESCRIPTOR_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_FILE_DESCRIPTOR_IS_NULL;
    }
    list_node_t *node = l->head;
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
    if (l == NULL)
    {
        list_error_no_custom_msg(LIST_E_LIST_IS_NULL, __FILE__, __func__, __LINE__);
        return LIST_E_LIST_IS_NULL;
    }
    if (coeffs == NULL)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "array of coefficients is NULL");
        return LIST_E_LIST_MANIPULATION;
    }
    if (l->size != size)
    {
        list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "size of the list does not match the expected size");
        return LIST_E_LIST_MANIPULATION;
    }
    list_node_t *node = l->head;
    for (size_t i = 0; i < size; i++)
    {
        if (node->coeff != coeffs[i])
        {
            char buffer[100];
            sprintf(buffer, "expected coefficient does not match the actual coefficient at position %zu", i);
            list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, buffer);
            return LIST_E_LIST_MANIPULATION;
        }
        node = node->next;
    }
    return LIST_E_SUCCESS;
}

list_t *list_create_from_array(uint8_t *coeffs, size_t size)
{
    list_t *lst = list_init();
    if (lst == NULL)
        return NULL;
    if (coeffs == NULL)
    {
        list_destroy(lst);
        return NULL;
    }
    for (size_t i = 0; i < size; i++)
    {
        if (list_add_end(lst, coeffs[i]) != LIST_E_SUCCESS)
        {
            if (list_destroy(lst) != LIST_E_SUCCESS)
            {
                list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "list_destroy() failed");
                return NULL;
            }
            list_error(LIST_E_LIST_MANIPULATION, __FILE__, __func__, __LINE__, "list_add_end() failed");
            return NULL;
        }
    }
    return lst;
}