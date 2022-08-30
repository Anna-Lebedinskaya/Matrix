#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int res = OK;
  if (rows < 1 || columns < 1) {
    res = INCORRECT_MATRIX;
  } else {
    result->matrix = (double **)calloc(rows, sizeof(double *));

    if (result->matrix == NULL) {
      res = INCORRECT_MATRIX;
    } else {
      for (int i = 0; i < rows; i++) {
        result->matrix[i] = (double *)calloc(columns, sizeof(double));
        if (result->matrix[i] == NULL) {
          res = INCORRECT_MATRIX;
          for (int j = 0; j < i; j++) {
            free(result->matrix[i]);
          }
          free(result->matrix);
          break;
        }
      }
      result->rows = rows;
      result->columns = columns;
    }
  }
  return res;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL && A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
  }
}

int check_matrix(matrix_t *A) {
  int check_res = 1;
  if (A == NULL || A->matrix == NULL || A->columns <= 0 || A->rows <= 0) {
    check_res = 0;
  }
  return check_res;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int res = SUCCESS;
  if (check_matrix(A) && check_matrix(B) && (A->columns == B->columns) &&
      (A->rows == B->rows)) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > S21_EPS) {
          res = FAILURE;
          break;
        }
      }
    }
  } else {
    res = FAILURE;
  }
  return res;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (check_matrix(A) == 0 || check_matrix(B) == 0 || A->matrix == NULL ||
      B->matrix == NULL) {
    res = INCORRECT_MATRIX;
  } else if ((A->columns == B->columns) && (A->rows == B->rows)) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  } else {
    res = CALCULATION_ERROR;
  }
  return res;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (check_matrix(A) == 0 || check_matrix(B) == 0 || A->matrix == NULL ||
      B->matrix == NULL) {
    res = INCORRECT_MATRIX;
  } else if ((A->columns == B->columns) && (A->rows == B->rows)) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  } else {
    res = CALCULATION_ERROR;
  }
  return res;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int res = OK;
  if (check_matrix(A) == 0 || A->matrix == NULL) {
    res = INCORRECT_MATRIX;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return res;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (check_matrix(A) == 0 || check_matrix(B) == 0 || A->matrix == NULL ||
      B->matrix == NULL) {
    res = INCORRECT_MATRIX;
  } else if (A->columns == B->rows) {
    s21_create_matrix(A->rows, B->columns, result);

    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        for (int k = 0; k < A->columns; k++) {
          result->matrix[i][j] =
              result->matrix[i][j] + A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  } else {
    res = CALCULATION_ERROR;
  }
  return res;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int res = OK;
  if (check_matrix(A) == 0) {  // result->matrix == NULL
    res = INCORRECT_MATRIX;
  } else {
    s21_create_matrix(A->columns, A->rows, result);

    for (int i = 0; i < A->columns; i++) {
      for (int j = 0; j < A->rows; j++) {
        result->matrix[i][j] = A->matrix[j][i];
      }
    }
  }
  return res;
}

matrix_t less_matrix(matrix_t *A, int num_row, int num_column) {
  matrix_t less_A;
  s21_create_matrix(A->rows - 1, A->columns - 1, &less_A);
  for (int i = 0; i < A->rows - 1; i++) {
    for (int j = 0; j < A->columns - 1; j++) {
      if (i < num_row && j < num_column) {
        less_A.matrix[i][j] = A->matrix[i][j];
      } else if (i >= num_row && j < num_column) {
        less_A.matrix[i][j] = A->matrix[i + 1][j];
      } else if (i >= num_row && j >= num_column) {
        less_A.matrix[i][j] = A->matrix[i + 1][j + 1];
      } else if (i < num_row && j >= num_column) {
        less_A.matrix[i][j] = A->matrix[i][j + 1];
      }
    }
  }
  return less_A;
}

double return_determinant(matrix_t *A) {
  double determinant = 0.0;
  if (A->rows == 1) {
    determinant = A->matrix[0][0];
  } else if (A->rows == 2) {
    determinant =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[1][0] * A->matrix[0][1];
  } else if (A->rows > 2) {
    for (int j = 0; j < A->columns; j++) {
      matrix_t less_A = less_matrix(A, 0, j);
      determinant = determinant +
                    A->matrix[0][j] * return_determinant(&less_A) * pow(-1, j);
      s21_remove_matrix(&less_A);
    }
  }
  return determinant;
}

int s21_determinant(matrix_t *A, double *result) {
  int res = OK;
  if (check_matrix(A) == 0 || A->matrix == NULL) {
    res = INCORRECT_MATRIX;
  } else if (A->columns != A->rows) {
    res = CALCULATION_ERROR;
  } else {
    *result = return_determinant(A);
  }
  return res;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  fflush(stdout);
  int res = OK;
  if (check_matrix(A) == 0 || A->matrix == NULL) {
    res = INCORRECT_MATRIX;
  } else if (A->rows == 1 || A->rows != A->columns) {
    res = CALCULATION_ERROR;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        matrix_t tmp = less_matrix(A, i, j);
        result->matrix[i][j] = return_determinant(&tmp) * pow(-1, i + j);
        s21_remove_matrix(&tmp);
      }
    }
  }
  return res;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int res = OK;
  double determinant = 0.0;
  s21_determinant(A, &determinant);
  if (check_matrix(A) == 0 || A->matrix == NULL) {
    res = INCORRECT_MATRIX;
  } else if (A->columns != A->rows || determinant == 0 || A->columns == 1) {
    res = CALCULATION_ERROR;
  } else {
    matrix_t complements_tmp;
    s21_calc_complements(A, &complements_tmp);

    matrix_t transpose_tmp;
    s21_transpose(&complements_tmp, &transpose_tmp);
    s21_mult_number(&transpose_tmp, 1 / determinant, result);

    s21_remove_matrix(&complements_tmp);
    s21_remove_matrix(&transpose_tmp);
  }
  return res;
}
