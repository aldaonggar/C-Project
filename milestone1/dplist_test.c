#define _GNU_SOURCE

#include "dplist.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct {
    int id;
    char* name;
} my_element_t;

void* element_copy(void * element);
void element_free(void ** element);
int element_compare(void * x, void * y);

my_element_t *character, *character1, *character2, *character3, *character4, *character5, *character6;


void * element_copy(void * element) {
    my_element_t* copy = malloc(sizeof (my_element_t));
    char* new_name;
    asprintf(&new_name,"%s",((my_element_t*)element)->name); //asprintf requires _GNU_SOURCE
    assert(copy != NULL);
    copy->id = ((my_element_t*)element)->id;
    copy->name = new_name;
    return (void *) copy;
}

void element_free(void ** element) {
    free((((my_element_t*)*element))->name);
    free(*element);
    *element = NULL;
}

int element_compare(void * x, void * y) {
    return ((((my_element_t*)x)->id < ((my_element_t*)y)->id) ? -1 : (((my_element_t*)x)->id == ((my_element_t*)y)->id) ? 0 : 1);
}

void setup(void) {
    character->id = 0;
    character->name = "F";
    character1->id = 1;
    character1->name = "A";
    character2->id = 2;
    character2->name = "B";
    character3->id = 3;
    character3->name = "C";
    character4->id = 4;
    character4->name = "G";
    character5->id = 5;
    character5->name = "D";
    character6->id = 6;
    character6->name = "E";
    return;
}

void teardown(void) {
    free(character);
    character = NULL;
    free(character1);
    character1 = NULL;
    free(character2);
    character2 = NULL;
    free(character3);
    character3 = NULL;
    free(character4);
    character4 = NULL;
    free(character5);
    character5 = NULL;
    free(character6);
    character6 = NULL;
    return;
}

START_TEST(test_ListFree)
    {
        // Test free NULL, don't use callback
        dplist_t *list = NULL;
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free NULL, use callback
        list = NULL;
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list, don't use callback
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list, use callback
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // TODO : Test free with one element, also test if inserted elements are set to NULL
        list = dpl_create(element_copy, element_free, element_compare);
        //dpl_insert_at_index(list, character, 0, false);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // TODO : Test free with multiple element, also test if inserted elements are set to NULL

    }
END_TEST

START_TEST(test_ListInsertAtIndexListNULL)
    {
        // Test inserting at index -1
        dplist_t *result = dpl_insert_at_index(NULL, character, -1, false);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        
        // TODO : Test inserting at index 0
        dpl_insert_at_index(result, character, 0, true);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        
        // TODO : Test inserting at index 99
        dpl_insert_at_index(result, character, 99, true);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
    }
END_TEST

START_TEST(test_ListInsertAtIndexListEmpty)
{
    // Test inserting at index -1
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);
    dplist_t *result = dpl_insert_at_index(list, character, -1, true);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list, true);
    
    // TODO : Test inserting at index 0
    list = dpl_create(element_copy, element_free, element_compare);
    result = dpl_insert_at_index(list, character, 0, false);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list, true);

    // TODO : Test inserting at index 99
    list = dpl_create(element_copy, element_free, element_compare);
    result = dpl_insert_at_index(list, character, 99, false);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list, true);

}
END_TEST

START_TEST(test_ListGetElementAtIndex)
{
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, character, 0, false);
    dpl_insert_at_index(list, character1, 1, false);
    dpl_insert_at_index(list, character2, 2, false);
    dpl_insert_at_index(list, character3, 3, false);
    dpl_insert_at_index(list, character4, 4, false);
    dpl_insert_at_index(list, character5, 5, true);
    dpl_insert_at_index(list, character6, 6, false);
    ck_assert_msg(dpl_get_element_at_index(list, 5) != character5, "Failure: expected result is not D");
    ck_assert_msg(dpl_get_element_at_index(list, 99) == character6, "Failure: expected result is E");
    ck_assert_msg(dpl_get_element_at_index(list, -1) == character, "Failure: expected result is F");
    ck_assert_msg(dpl_get_element_at_index(list, 4) == character4, "Failure: expected result is G");
    dpl_free(&list, true);
    
}
END_TEST

START_TEST(test_ListGetListWithRemovedElementAtIndex)
{
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, character, 0, false);
    dpl_insert_at_index(list, character1, 1, false);
    dpl_insert_at_index(list, character2, 2, false);
    dpl_insert_at_index(list, character3, 3, false);
    dpl_insert_at_index(list, character4, 4, false);
    dpl_insert_at_index(list, character5, 5, false);
    dpl_insert_at_index(list, character6, 6, false);
    dpl_remove_at_index(list, 5, true);
    ck_assert_msg(dpl_get_element_at_index(list, 5) == character6, "Failure: expected result is E");
    dpl_remove_at_index(list, 99, true);
    ck_assert_msg(dpl_get_element_at_index(list, 99) == character4, "Failure: expected result is G");
    dpl_remove_at_index(list, -1, true);
    ck_assert_msg(dpl_get_element_at_index(list, -1) == character1, "Failure: expected result is A");
    dpl_free(&list, true);
    
}
END_TEST

START_TEST(test_ListGetIndexOfElement)
{
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, character, 0, false);
    dpl_insert_at_index(list, character1, 1, false);
    dpl_insert_at_index(list, character2, 2, false);
    dpl_insert_at_index(list, character3, 3, false);
    dpl_insert_at_index(list, character4, 4, false);
    dpl_insert_at_index(list, character5, 5, false);
    dpl_insert_at_index(list, character6, 6, false);
    my_element_t* k;
    k->id = 7;
    k->name = "K";
    ck_assert_msg(dpl_get_index_of_element(list, character1) == 1, "Failure: expected result is 1");
    ck_assert_msg(dpl_get_index_of_element(list, character6) == 6, "Failure: expected result is 6");
    ck_assert_msg(dpl_get_index_of_element(list, character4) == 4, "Failure: expected result is 4");
    ck_assert_msg(dpl_get_index_of_element(list, k) == -1, "Failure: expected result is -1");
    dpl_free(&list, true);
    
}
END_TEST

int main(void) {
    Suite *s1 = suite_create("LIST_EX3");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_checked_fixture(tc1_1, setup, teardown);
    tcase_add_test(tc1_1, test_ListFree);
    tcase_add_test(tc1_1, test_ListInsertAtIndexListNULL);
    tcase_add_test(tc1_1, test_ListInsertAtIndexListEmpty);
    tcase_add_test(tc1_1, test_ListGetElementAtIndex);
    tcase_add_test(tc1_1, test_ListGetListWithRemovedElementAtIndex);
    tcase_add_test(tc1_1, test_ListGetIndexOfElement);

    srunner_run_all(sr, CK_VERBOSE);

    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
