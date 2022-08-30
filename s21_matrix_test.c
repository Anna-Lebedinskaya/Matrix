// gcc -o test s21_matrix_test.c s21_matrix.c -lcheck -lm -lpthread && ./test
// leaks -atExit -- ./test
// valgrind --leak-check=full --show-leak-kinds=all -s ./test
#include "s21_matrix.h"

#include <check.h>

START_TEST(test_s21_create) {
  matrix_t result;
  int test = s21_create_matrix(0, 0, &result);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  test = s21_create_matrix(10, 20, &result);
  ck_assert_int_eq(test, OK);
  s21_remove_matrix(&result);

  test = s21_create_matrix(-2, 2, &result);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  test = s21_create_matrix(0, 200, &result);
  ck_assert_int_eq(test, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_s21_eq) {
  matrix_t test1, test2;

  s21_create_matrix(1, 1, &test1);
  s21_create_matrix(1, 1, &test2);
  test1.matrix[0][0] = 1;
  int res = s21_eq_matrix(&test1, &test2);
  ck_assert_int_eq(res, FAILURE);
  s21_remove_matrix(&test1);
  s21_remove_matrix(&test2);

  s21_create_matrix(1, 1, &test1);
  s21_create_matrix(1, 1, &test2);
  res = s21_eq_matrix(&test1, &test2);
  ck_assert_int_eq(res, SUCCESS);
  s21_remove_matrix(&test1);
  s21_remove_matrix(&test2);

  s21_create_matrix(1, 2, &test1);
  s21_create_matrix(1, 1, &test2);
  res = s21_eq_matrix(&test1, &test2);
  ck_assert_int_eq(res, FAILURE);
  s21_remove_matrix(&test1);
  s21_remove_matrix(&test2);

  s21_create_matrix(2, 3, &test1);
  s21_create_matrix(2, 3, &test2);
  double count = 2.5;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      test1.matrix[i][j] = count;
      count = count + 2.5;
    }
  }
  count = 2.50000001;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      test2.matrix[i][j] = count;
      count = count + 2.5;
    }
  }
  res = s21_eq_matrix(&test1, &test2);
  ck_assert_int_eq(res, SUCCESS);
  s21_remove_matrix(&test1);
  s21_remove_matrix(&test2);
}
END_TEST

START_TEST(test_s21_matrix_sum) {
  matrix_t test1, test2, result1, result2;

  s21_create_matrix(3, 3, &test1);
  s21_create_matrix(3, 3, &test2);
  s21_create_matrix(3, 3, &result2);
  test1.matrix[0][0] = 1;
  test2.matrix[0][0] = 2;
  result2.matrix[0][0] = 3;
  int res_sum = s21_sum_matrix(&test1, &test2, &result1);
  ck_assert_int_eq(res_sum, OK);
  int res_eq = s21_eq_matrix(&result1, &result2);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&test1);
  s21_remove_matrix(&test2);
  s21_remove_matrix(&result1);
  s21_remove_matrix(&result2);

  test1.rows = -1;
  res_sum = s21_sum_matrix(&test1, &test2, &result1);
  ck_assert_int_eq(res_sum, INCORRECT_MATRIX);

  s21_create_matrix(1, 1, &test1);
  s21_create_matrix(1, 2, &test2);
  res_sum = s21_sum_matrix(&test1, &test2, &result1);
  ck_assert_int_eq(res_sum, CALCULATION_ERROR);

  s21_remove_matrix(&test1);
  s21_remove_matrix(&test2);
}
END_TEST

START_TEST(test_s21_matrix_sub) {
  matrix_t test1, test2, result1, result2;

  s21_create_matrix(3, 3, &test1);
  s21_create_matrix(3, 3, &test2);
  s21_create_matrix(3, 3, &result2);
  test1.matrix[0][0] = 5;
  test2.matrix[0][0] = 2;
  result2.matrix[0][0] = 3;
  int res_sub = s21_sub_matrix(&test1, &test2, &result1);
  ck_assert_int_eq(res_sub, OK);
  int res_eq = s21_eq_matrix(&result1, &result2);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&test1);
  s21_remove_matrix(&test2);
  s21_remove_matrix(&result1);
  s21_remove_matrix(&result2);

  test1.rows = -1;
  res_sub = s21_sub_matrix(&test1, &test2, &result1);
  ck_assert_int_eq(res_sub, INCORRECT_MATRIX);

  s21_create_matrix(1, 1, &test1);
  s21_create_matrix(1, 2, &test2);
  res_sub = s21_sub_matrix(&test1, &test2, &result1);
  ck_assert_int_eq(res_sub, CALCULATION_ERROR);
  s21_remove_matrix(&test1);
  s21_remove_matrix(&test2);
}
END_TEST

START_TEST(test_s21_matrix_mult_number) {
  matrix_t test1, result1, result2;
  double number = 5.0;
  s21_create_matrix(3, 3, &test1);
  test1.matrix[0][0] = 1;
  s21_create_matrix(3, 3, &result2);
  result2.matrix[0][0] = 5.0;
  int res_mult = s21_mult_number(&test1, number, &result1);
  ck_assert_int_eq(res_mult, OK);
  int res_eq = s21_eq_matrix(&result1, &result2);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&result1);
  s21_remove_matrix(&test1);
  s21_remove_matrix(&result2);

  test1.rows = -1;
  res_mult = s21_mult_number(&test1, number, &result1);
  ck_assert_int_eq(res_mult, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_s21_matrix_mult_matrix) {
  matrix_t test1, test2, result1, result2;

  s21_create_matrix(4, 3, &test1);
  s21_create_matrix(1, 2, &test2);
  int res_mult = s21_mult_matrix(&test1, &test2, &result1);
  ck_assert_int_eq(res_mult, CALCULATION_ERROR);
  s21_remove_matrix(&test1);
  s21_remove_matrix(&test2);

  s21_create_matrix(2, 1, &test1);
  s21_create_matrix(1, 2, &test2);
  s21_create_matrix(2, 2, &result2);
  test1.matrix[0][0] = 2;
  test1.matrix[1][0] = 4;
  test2.matrix[0][0] = 1;
  test2.matrix[0][1] = 3;
  result2.matrix[0][0] = 2;
  result2.matrix[0][1] = 6;
  result2.matrix[1][0] = 4;
  result2.matrix[1][1] = 12;
  res_mult = s21_mult_matrix(&test1, &test2, &result1);
  int res_eq = s21_eq_matrix(&result1, &result2);
  ck_assert_int_eq(res_mult, OK);
  ck_assert_int_eq(res_eq, SUCCESS);

  s21_remove_matrix(&test1);
  s21_remove_matrix(&test2);
  s21_remove_matrix(&result1);
  s21_remove_matrix(&result2);

  test1.rows = -1;
  res_mult = s21_mult_matrix(&test1, &test2, &result1);
  ck_assert_int_eq(res_mult, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_s21_transpose) {
  matrix_t test1 = {0}, result1, result2;

  test1.rows = -1;

  int res_transp = s21_transpose(&test1, &result1);
  ck_assert_int_eq(res_transp, INCORRECT_MATRIX);

  s21_create_matrix(1, 3, &test1);
  s21_create_matrix(3, 1, &result2);
  test1.matrix[0][0] = 1;
  test1.matrix[0][1] = 2;
  test1.matrix[0][2] = 3;
  result2.matrix[0][0] = 1;
  result2.matrix[1][0] = 2;
  result2.matrix[2][0] = 3;
  res_transp = s21_transpose(&test1, &result1);
  int res_eq = s21_eq_matrix(&result1, &result2);
  ck_assert_int_eq(res_transp, OK);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&test1);
  s21_remove_matrix(&result1);
  s21_remove_matrix(&result2);

  s21_create_matrix(2, 2, &test1);
  s21_create_matrix(2, 2, &result2);
  test1.matrix[0][0] = 1;
  test1.matrix[0][1] = 2;
  test1.matrix[1][0] = 3;
  test1.matrix[1][1] = 4;
  result2.matrix[0][0] = 1;
  result2.matrix[0][1] = 3;
  result2.matrix[1][0] = 2;
  result2.matrix[1][1] = 4;
  res_transp = s21_transpose(&test1, &result1);
  res_eq = s21_eq_matrix(&result1, &result2);
  ck_assert_int_eq(res_transp, 0);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&test1);
  s21_remove_matrix(&result1);
  s21_remove_matrix(&result2);
}
END_TEST

START_TEST(test_s21_calc_complements) {
  matrix_t test = {0}, result1, result2;
  int res_compl, res_eq;

  test.rows = -1;
  res_compl = s21_calc_complements(&test, &result1);
  ck_assert_int_eq(res_compl, INCORRECT_MATRIX);

  s21_create_matrix(1, 1, &test);
  test.matrix[0][0] = 5;
  res_compl = s21_calc_complements(&test, &result1);
  ck_assert_int_eq(res_compl, CALCULATION_ERROR);
  s21_remove_matrix(&test);

  s21_create_matrix(3, 3, &test);
  s21_create_matrix(3, 3, &result2);
  test.matrix[0][0] = 1;
  test.matrix[0][1] = 2;
  test.matrix[0][2] = 3;
  test.matrix[1][0] = 0;
  test.matrix[1][1] = 4;
  test.matrix[1][2] = 2;
  test.matrix[2][0] = 5;
  test.matrix[2][1] = 2;
  test.matrix[2][2] = 1;
  result2.matrix[0][0] = 0;
  result2.matrix[0][1] = 10;
  result2.matrix[0][2] = -20;
  result2.matrix[1][0] = 4;
  result2.matrix[1][1] = -14;
  result2.matrix[1][2] = 8;
  result2.matrix[2][0] = -8;
  result2.matrix[2][1] = -2;
  result2.matrix[2][2] = 4;
  res_compl = s21_calc_complements(&test, &result1);
  res_eq = s21_eq_matrix(&result1, &result2);
  ck_assert_int_eq(res_compl, 0);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&test);
  s21_remove_matrix(&result1);
  s21_remove_matrix(&result2);
}
END_TEST

START_TEST(test_s21_determinant) {
  matrix_t test = {0};
  double result;
  int res_determ;
  test.rows = -1;
  res_determ = s21_determinant(&test, &result);
  ck_assert_int_eq(res_determ, INCORRECT_MATRIX);

  s21_create_matrix(2, 4, &test);
  res_determ = s21_determinant(&test, &result);
  ck_assert_int_eq(res_determ, CALCULATION_ERROR);
  s21_remove_matrix(&test);

  s21_create_matrix(1, 1, &test);
  res_determ = s21_determinant(&test, &result);
  ck_assert_int_eq(res_determ, OK);
  s21_remove_matrix(&test);

  s21_create_matrix(2, 2, &test);
  test.matrix[0][0] = 1;
  test.matrix[0][1] = 2;
  test.matrix[1][0] = 3;
  test.matrix[1][1] = 4;
  res_determ = s21_determinant(&test, &result);
  ck_assert_int_eq(res_determ, OK);
  ck_assert_int_eq(result, -2);
  s21_remove_matrix(&test);

  s21_create_matrix(3, 3, &test);
  test.matrix[0][0] = 1;
  test.matrix[0][1] = 2;
  test.matrix[0][2] = 3;
  test.matrix[1][0] = 4;
  test.matrix[1][1] = 5;
  test.matrix[1][2] = 6;
  test.matrix[2][0] = 7;
  test.matrix[2][1] = 8;
  test.matrix[2][2] = 9;
  res_determ = s21_determinant(&test, &result);
  ck_assert_int_eq(res_determ, 0);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&test);

  s21_create_matrix(4, 4, &test);
  test.matrix[0][0] = 1;
  test.matrix[0][1] = 2;
  test.matrix[0][2] = 3;
  test.matrix[0][3] = 4;
  test.matrix[1][0] = 5;
  test.matrix[1][1] = 6;
  test.matrix[1][2] = 7;
  test.matrix[1][3] = 8;
  test.matrix[2][0] = 9;
  test.matrix[2][1] = 10;
  test.matrix[2][2] = 11;
  test.matrix[2][3] = 12;
  test.matrix[3][0] = 13;
  test.matrix[3][1] = 14;
  test.matrix[3][2] = 15;
  test.matrix[3][3] = 16;
  res_determ = s21_determinant(&test, &result);
  ck_assert_int_eq(res_determ, 0);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&test);
}
END_TEST

START_TEST(test_s21_inverse) {
  matrix_t test = {0}, result1, result2;
  int res_inverse, res_eq;
  test.rows = -1;
  res_inverse = s21_inverse_matrix(&test, &result1);
  ck_assert_int_eq(res_inverse, INCORRECT_MATRIX);

  s21_create_matrix(1, 1, &test);
  test.matrix[0][0] = 0;
  res_inverse = s21_inverse_matrix(&test, &result1);
  ck_assert_int_eq(res_inverse, CALCULATION_ERROR);
  s21_remove_matrix(&test);

  s21_create_matrix(2, 2, &test);
  s21_create_matrix(2, 2, &result2);
  test.matrix[0][0] = 1;
  test.matrix[0][1] = 2;
  test.matrix[1][0] = -1;
  test.matrix[1][1] = 2;
  result2.matrix[0][0] = 0.5;
  result2.matrix[0][1] = -0.5;
  result2.matrix[1][0] = 0.25;
  result2.matrix[1][1] = 0.25;
  res_inverse = s21_inverse_matrix(&test, &result1);
  res_eq = s21_eq_matrix(&result1, &result2);
  ck_assert_int_eq(res_inverse, OK);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&test);
  s21_remove_matrix(&result1);
  s21_remove_matrix(&result2);

  s21_create_matrix(3, 3, &test);
  s21_create_matrix(3, 3, &result2);
  test.matrix[0][0] = 2;
  test.matrix[0][1] = 5;
  test.matrix[0][2] = 7;
  test.matrix[1][0] = 6;
  test.matrix[1][1] = 3;
  test.matrix[1][2] = 4;
  test.matrix[2][0] = 5;
  test.matrix[2][1] = -2;
  test.matrix[2][2] = -3;
  result2.matrix[0][0] = 1;
  result2.matrix[0][1] = -1;
  result2.matrix[0][2] = 1;
  result2.matrix[1][0] = -38;
  result2.matrix[1][1] = 41;
  result2.matrix[1][2] = -34;
  result2.matrix[2][0] = 27;
  result2.matrix[2][1] = -29;
  result2.matrix[2][2] = 24;
  res_inverse = s21_inverse_matrix(&test, &result1);
  res_eq = s21_eq_matrix(&result1, &result2);
  ck_assert_int_eq(res_inverse, 0);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&test);
  s21_remove_matrix(&result1);
  s21_remove_matrix(&result2);
}
END_TEST

int main() {
  Suite *s = suite_create("s21_matrix_tests");
  TCase *tc_core = tcase_create("Core");
  SRunner *sr = srunner_create(s);
  int number_failed;

  suite_add_tcase(s, tc_core);

  tcase_add_test(tc_core, test_s21_create);
  tcase_add_test(tc_core, test_s21_eq);
  tcase_add_test(tc_core, test_s21_matrix_sum);
  tcase_add_test(tc_core, test_s21_matrix_sub);
  tcase_add_test(tc_core, test_s21_matrix_mult_number);
  tcase_add_test(tc_core, test_s21_matrix_mult_matrix);
  tcase_add_test(tc_core, test_s21_transpose);
  tcase_add_test(tc_core, test_s21_calc_complements);
  tcase_add_test(tc_core, test_s21_determinant);
  tcase_add_test(tc_core, test_s21_inverse);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
