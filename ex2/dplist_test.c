/**
 * \author Nur Alda-Onggar
 */

#include "dplist.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>

char* character = "F";
char* character1 = "A";
char* character2 = "B";
char* character3 = "C";
char* character4 = "G";
char* character5 = "D";
char* character6 = "E";

void setup(void) {
    // Implement pre-test setup
}

void teardown(void) {
    // Implement post-test teardown
}

START_TEST(test_ListFree)
    {
        // Test free NULL
        dplist_t *list = NULL;
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list
        list = dpl_create();
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // TODO : Test free with one element
        list = dpl_create();
        dpl_insert_at_index(list, character, 0);
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // TODO : Test free with multiple element
        list = dpl_create();
        dpl_insert_at_index(list, character, 0);
        dpl_insert_at_index(list, character1, 1);
        dpl_insert_at_index(list, character2, 2);
        dpl_insert_at_index(list, character3, 3);
        dpl_insert_at_index(list, character4, 4);
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

    }
END_TEST

START_TEST(test_ListInsertAtIndexListNULL)
    {
        // Test inserting at index -1
        dplist_t *result = dpl_insert_at_index(NULL, character1, -1);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        
        // TODO : Test inserting at index 0
        dpl_insert_at_index(result, character1, 0);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        
        // TODO : Test inserting at index 99
        dpl_insert_at_index(result, character1, 99);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
    }
END_TEST

START_TEST(test_ListInsertAtIndexListEmpty)
{
    // Test inserting at index -1
    dplist_t *list = dpl_create();
    dplist_t *result = dpl_insert_at_index(list, character1, -1);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list);
    
    // TODO : Test inserting at index 0
    list = dpl_create();
    result = dpl_insert_at_index(list, character1, 0);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list);

    // TODO : Test inserting at index 99
    list = dpl_create();
    result = dpl_insert_at_index(list, character1, 99);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list);

}
END_TEST

START_TEST(test_ListGetElementAtIndex)
{
    dplist_t *list = dpl_create();
    dpl_insert_at_index(list, character, 0);
    dpl_insert_at_index(list, character1, 1);
    dpl_insert_at_index(list, character2, 2);
    dpl_insert_at_index(list, character3, 3);
    dpl_insert_at_index(list, character4, 4);
    dpl_insert_at_index(list, character5, 5);
    dpl_insert_at_index(list, character6, 6);
    ck_assert_msg(dpl_get_element_at_index(list, 5) == character5, "Failure: expected result is D");
    ck_assert_msg(dpl_get_element_at_index(list, 99) == character6, "Failure: expected result is E");
    ck_assert_msg(dpl_get_element_at_index(list, -1) == character, "Failure: expected result is F");
    ck_assert_msg(dpl_get_element_at_index(list, 4) == character4, "Failure: expected result is G");
    dpl_free(&list);
    
}
END_TEST

START_TEST(test_ListGetListWithRemovedElementAtIndex)
{
    dplist_t *list = dpl_create();
    dpl_insert_at_index(list, character, 0);
    dpl_insert_at_index(list, character1, 1);
    dpl_insert_at_index(list, character2, 2);
    dpl_insert_at_index(list, character3, 3);
    dpl_insert_at_index(list, character4, 4);
    dpl_insert_at_index(list, character5, 5);
    dpl_insert_at_index(list, character6, 6);
    dpl_remove_at_index(list, 5);
    ck_assert_msg(dpl_get_element_at_index(list, 5) == character6, "Failure: expected result is E");
    dpl_remove_at_index(list, 99);
    ck_assert_msg(dpl_get_element_at_index(list, 99) == character4, "Failure: expected result is G");
    dpl_remove_at_index(list, -1);
    ck_assert_msg(dpl_get_element_at_index(list, -1) == character1, "Failure: expected result is A");
    dpl_free(&list);
    
}
END_TEST

START_TEST(test_ListGetIndexOfElement)
{
    dplist_t *list = dpl_create();
    dpl_insert_at_index(list, character, 0);
    dpl_insert_at_index(list, character1, 1);
    dpl_insert_at_index(list, character2, 2);
    dpl_insert_at_index(list, character3, 3);
    dpl_insert_at_index(list, character4, 4);
    dpl_insert_at_index(list, character5, 5);
    dpl_insert_at_index(list, character6, 6);
    char* k = "K";
    ck_assert_msg(dpl_get_index_of_element(list, character1) == 1, "Failure: expected result is 1");
    ck_assert_msg(dpl_get_index_of_element(list, character6) == 6, "Failure: expected result is 6");
    ck_assert_msg(dpl_get_index_of_element(list, character4) == 4, "Failure: expected result is 4");
    ck_assert_msg(dpl_get_index_of_element(list, k) == -1, "Failure: expected result is -1");
    dpl_free(&list);
    
}
END_TEST


int main(void) {
    Suite *s1 = suite_create("LIST_EX1");
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
