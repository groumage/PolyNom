#include "../include/list.h"
#include <assert.h>
#include <string.h>

int main()
{
    // nominal cases
    list_t *list = list_init();
    assert (list_add_beginning(list, 1) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {1}, 1) == LIST_E_SUCCESS);
    assert (list_add_beginning(list, 2) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {2, 1}, 2) == LIST_E_SUCCESS);
    assert (list_add_end(list, 3) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {2, 1, 3}, 3) == LIST_E_SUCCESS);
    assert (list_add_at(list, 4, 0) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {4, 2, 1, 3}, 4) == LIST_E_SUCCESS);
    assert (list_add_at(list, 5, 2) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {4, 2, 5, 1, 3}, 5) == LIST_E_SUCCESS);
    assert (list_add_at(list, 6, 5) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {4, 2, 5, 1, 3, 6}, 6) == LIST_E_SUCCESS);
    assert (list_add_end(list, 3) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {4, 2, 5, 1, 3, 6, 3}, 7) == LIST_E_SUCCESS);
    assert (list_remove_coeff(list, 3) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {4, 2, 5, 1, 6, 3}, 6) == LIST_E_SUCCESS);
    assert (list_remove_coeff(list, 4) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {2, 5, 1, 6, 3}, 5) == LIST_E_SUCCESS);
    assert (list_add_beginning(list, 4) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {4, 2, 5, 1, 6, 3}, 6) == LIST_E_SUCCESS);
    assert (list_remove_node(list, list->head->next) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {4, 5, 1, 6, 3}, 5) == LIST_E_SUCCESS);
    assert (list_remove_node(list, list->head) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {5, 1, 6, 3}, 4) == LIST_E_SUCCESS);
    assert (list_remove_node(list, list->tail) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {5, 1, 6}, 3) == LIST_E_SUCCESS);
    assert (list_add_end(list, 8) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {5, 1, 6, 8},4) == LIST_E_SUCCESS);
    assert (list_remove_head(list) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {1, 6, 8}, 3) == LIST_E_SUCCESS);
    assert (list_remove_tail(list) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {1, 6}, 2) == LIST_E_SUCCESS);
    assert (list_remove_coeff(list, 6) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {1}, 1) == LIST_E_SUCCESS);
    assert (list_add_after(list, 2, list->head) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {1, 2}, 2) == LIST_E_SUCCESS);
    assert (list_get_at_pos(list, 0)->coeff == 1);
    assert (list_get_at_pos(list, 1)->coeff == 2);
    assert (list_add_after(list, 3, list->head) == LIST_E_SUCCESS);
    assert (list_assert(list, (uint8_t []) {1, 3, 2}, 3) == LIST_E_SUCCESS);
    assert (list_get_pos(list, list->head) == 0);
    assert (list_get_pos(list, list->head->next->next) == 2);
    assert (list_destroy(list) == LIST_E_SUCCESS);

    // error cases: null pointers
    assert (list_destroy(NULL) == LIST_E_NULL_PTR);
    assert (list_add_beginning(NULL, 0) == LIST_E_NULL_PTR);
    assert (list_add_end(NULL, 0) == LIST_E_NULL_PTR);
    assert (list_remove_node(NULL, NULL) == LIST_E_NULL_PTR);
    assert (list_remove_node(NULL, 0) == LIST_E_NULL_PTR);
    assert (list_remove_head(NULL) == LIST_E_NULL_PTR);
    assert (list_remove_tail(NULL) == LIST_E_NULL_PTR);
    assert (list_add_at(NULL, 0, 0) == LIST_E_NULL_PTR);
    assert (list_print(NULL, NULL) == LIST_E_NULL_PTR);
    assert (list_assert(NULL, (uint8_t []) {1}, 1) == LIST_E_NULL_PTR);
    assert (list_create_from_array(NULL, 0) == NULL);
    assert (list_add_after(NULL, 0, NULL) == LIST_E_NULL_PTR);
    assert (list_get_pos(NULL, NULL) == LIST_E_NULL_PTR);
    assert (list_get_at_pos(NULL, 0) == NULL);

    // error cases: other
    list = list_init();
    assert (list_remove_coeff(list, 0) == LIST_E_EMPTY);
    assert (list_remove_node(list, 0) == LIST_E_EMPTY);
    assert (list_remove_head(list) == LIST_E_EMPTY);
    assert (list_remove_tail(list) == LIST_E_EMPTY);
    assert (list_add_beginning(list, 1) == LIST_E_SUCCESS);
    assert (list_add_at(list, 2, 3) == LIST_E_INVALID_ARGUMENT);
    assert (list_get_pos(list, NULL) == LIST_E_INVALID_ARGUMENT);
    assert (list_remove_coeff(NULL, 0) == LIST_E_NULL_PTR);
    assert (list_remove_coeff(list, 2) == LIST_E_NOT_FOUND);
    assert (list_assert(list, NULL, 1) == LIST_E_INVALID_ARGUMENT);
    assert (list_assert(list, (uint8_t []) {2}, 1) == LIST_E_INVALID_ARGUMENT);
    assert (list_add_after(list, 0, NULL) == LIST_E_INVALID_ARGUMENT);
    list_t *list2 = list_init();
    assert (list_add_beginning(list2, 1) == LIST_E_SUCCESS);
    assert (list_assert(list2, (uint8_t []) {1}, 2) == LIST_E_INVALID_ARGUMENT);
    assert (list_get_pos(list2, list->head) == LIST_E_NOT_FOUND);
    assert (list_remove_node(list, NULL) == LIST_E_INVALID_ARGUMENT);
    assert (list_remove_node(list, list2->head) == LIST_E_NOT_FOUND);
    assert (list_remove_tail(list2) == LIST_E_NOT_FOUND);
    assert (list_get_at_pos(list, 2) == NULL);
    assert (list_destroy(list) == LIST_E_SUCCESS);
    assert (list_destroy(list2) == LIST_E_SUCCESS);

    // print case
    list = list_init();
    assert (list_add_beginning(list, 1) == LIST_E_SUCCESS);
    assert (list_add_beginning(list, 2) == LIST_E_SUCCESS);
    assert (list_add_beginning(list, 3) == LIST_E_SUCCESS);
    char filename[] = "list_output.txt";
    FILE* fp = fopen(filename, "w");
    assert (list_print(NULL, list) == LIST_E_FD_IS_NULL);
    assert (list_print(fp, list) == LIST_E_SUCCESS);
    fclose(fp);
    fp = fopen(filename, "r");
    char buffer[] = "0 0 0 \n";
    fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);
    char expected_output[] = "3 2 1 \n";
    assert (strcmp(buffer, expected_output) == 0);
    assert (list_destroy(list) == LIST_E_SUCCESS);
    remove(filename);
    return 0;
}