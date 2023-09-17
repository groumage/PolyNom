#include "../../lib/include/list.h"

int main()
{
    list_t list = {NULL, NULL, 0};
    list_node_t tail = {2, NULL};
    list_node_t mid = {1, &tail};
    list_node_t head = {0, &mid};
    (&list)->head = &head;
    (&list)->tail = &tail;
    (&list)->size = 2;
    assert (list_init() == NULL);
    assert (list_add_beginning(&list, 0) == LIST_E_MEMORY);
    assert (list_add_end(&list, 0) == LIST_E_MEMORY);
    assert (list_add_at(&list, 5, 1) == LIST_E_MEMORY);
    assert (list_add_after(&list, 5, (&list)->head) == LIST_E_MEMORY);
    assert (list_create_from_array(NULL, 0) == NULL);
    //assert (list_create_from_array((uint8_t[]){0}, 1) == NULL);
    return 0;
}