// g++ s21_matrix_oop_test.cpp s21_matrix_oop.cpp -o test -lgtest -lgtest_main
// -pthread -g && ./test
#include "s21_matrix_oop.h"

#include <gtest/gtest.h>

void FillMatrix(S21Matrix &A);

TEST(CreateNewObject, test1) {
  S21Matrix A;
  ASSERT_EQ(A.GetRows(), 2);
  ASSERT_EQ(A.GetCols(), 2);

  S21Matrix B(4, 5);
  ASSERT_EQ(B.GetRows(), 4);
  ASSERT_EQ(B.GetCols(), 5);

  ASSERT_ANY_THROW(S21Matrix C(1, 0));
  ASSERT_ANY_THROW(S21Matrix C(-2, 1));
}

TEST(CopyObject, test1) {
  S21Matrix A(5, 5);
  FillMatrix(A);
  S21Matrix B(A);

  for (int i = 0; i < 1; i++) {
    for (int j = 0; j < 1; j++) {
      ASSERT_DOUBLE_EQ(A(i, j), B(i, j));
    }
  }
}

TEST(Set, test1) {
  S21Matrix A(5, 5);
  FillMatrix(A);
  A.SetRowsCols(2, 0);
  ASSERT_EQ(A.GetRows(), 2);
  A.SetRowsCols(3, 1);
  ASSERT_EQ(A.GetCols(), 3);
  ASSERT_ANY_THROW(A.SetRowsCols(-2, 1));
}

TEST(Move, test1) {
  S21Matrix A(2, 2);
  FillMatrix(A);

  S21Matrix B(2, 2);
  FillMatrix(B);

  S21Matrix C = std::move(A);

  ASSERT_EQ(B(0, 0), C(0, 0));
  ASSERT_EQ(B(0, 1), C(0, 1));
  ASSERT_EQ(B(1, 0), C(1, 0));
  ASSERT_EQ(B(0, 1), C(0, 1));
}

TEST(EqMatrix, test1) {
  S21Matrix A(2, 2);
  A(0, 0) = 1;
  A(0, 1) = 3;
  A(1, 0) = 5;
  A(1, 0) = -1;

  S21Matrix B(2, 2);
  B(0, 0) = 1;
  B(0, 1) = 3;
  B(1, 0) = 5;
  B(1, 0) = -1;

  ASSERT_TRUE(A.EqMatrix(B));

  ASSERT_TRUE(A.EqMatrix(A));

  B(0, 0) = 0;
  ASSERT_FALSE(A.EqMatrix(B));
}

TEST(SumMatrix, test1) {
  S21Matrix A(2, 2);
  A(0, 0) = 1;
  A(0, 1) = 3;
  A(1, 0) = 5;
  A(1, 0) = -1;

  S21Matrix B(2, 2);
  B(0, 0) = 1;
  B(0, 1) = 3;
  B(1, 0) = 5;
  B(1, 0) = -1;

  S21Matrix C = A + B;

  A.SumMatrix(B);

  ASSERT_EQ(A(0, 0), C(0, 0));
  ASSERT_EQ(A(0, 1), C(0, 1));
  ASSERT_EQ(A(1, 0), C(1, 0));
  ASSERT_EQ(A(0, 1), C(0, 1));

  A = B;
  B(0, 0) = 0;
  A.SumMatrix(B);
  ASSERT_FALSE(A(0, 0) == C(0, 0));

  A = B;
  A.SetRowsCols(1, 0);
  ASSERT_ANY_THROW(A.SumMatrix(B));

  A = B;
  C = A;
  A += B;
  C.SumMatrix(B);

  ASSERT_EQ(A(0, 0), C(0, 0));
  ASSERT_EQ(A(0, 1), C(0, 1));
  ASSERT_EQ(A(1, 0), C(1, 0));
  ASSERT_EQ(A(0, 1), C(0, 1));
}

TEST(SubMatrix, test1) {
  S21Matrix A(2, 2);
  A(0, 0) = 1;
  A(0, 1) = 3;
  A(1, 0) = 5;
  A(1, 0) = -1;

  S21Matrix B(2, 2);
  B(0, 0) = 1;
  B(0, 1) = 3;
  B(1, 0) = 5;
  B(1, 0) = -1;

  S21Matrix C = A - B;

  A.SubMatrix(B);

  ASSERT_EQ(A(0, 0), C(0, 0));
  ASSERT_EQ(A(0, 1), C(0, 1));
  ASSERT_EQ(A(1, 0), C(1, 0));
  ASSERT_EQ(A(0, 1), C(0, 1));

  A = B;
  B(0, 0) = 0;
  A.SumMatrix(B);
  ASSERT_FALSE(A(0, 0) == C(0, 0));

  A = B;
  A.SetRowsCols(1, 0);
  ASSERT_ANY_THROW(A.SubMatrix(B));
}

TEST(SubMatrix, test2) {
  S21Matrix A(2, 2);
  A(0, 0) = 1;
  A(0, 1) = 3;
  A(1, 0) = 5;
  A(1, 0) = -1;

  S21Matrix B(2, 2);
  B(0, 0) = 1;
  B(0, 1) = 3;
  B(1, 0) = 5;
  B(1, 0) = -1;

  S21Matrix C(2, 2);
  C(0, 0) = 1;
  C(0, 1) = 3;
  C(1, 0) = 5;
  C(1, 0) = -1;

  C -= B;

  A.SubMatrix(B);

  ASSERT_EQ(A(0, 0), C(0, 0));
  ASSERT_EQ(A(0, 1), C(0, 1));
  ASSERT_EQ(A(1, 0), C(1, 0));
  ASSERT_EQ(A(0, 1), C(0, 1));
}

TEST(MulNumber, test1) {
  S21Matrix A(2, 2);
  A(0, 0) = 1;
  A(0, 1) = 3;
  A(1, 0) = 5;
  A(1, 0) = -1;

  S21Matrix C = A * 10;

  A.MulNumber(10);

  ASSERT_EQ(A(0, 0), C(0, 0));
  ASSERT_EQ(A(0, 1), C(0, 1));
  ASSERT_EQ(A(1, 0), C(1, 0));
  ASSERT_EQ(A(0, 1), C(0, 1));
  ASSERT_TRUE(A == C);
}

TEST(MulNumber, test2) {
  S21Matrix A(2, 2);
  A(0, 0) = 1;
  A(0, 1) = 3;
  A(1, 0) = 5;
  A(1, 0) = -1;

  S21Matrix C(2, 2);
  C(0, 0) = -1;
  C(0, 1) = -3;
  C(1, 0) = -5;
  C(1, 0) = 1;
  C *= -10;

  A.MulNumber(10);

  ASSERT_EQ(A(0, 0), C(0, 0));
  ASSERT_EQ(A(0, 1), C(0, 1));
  ASSERT_EQ(A(1, 0), C(1, 0));
  ASSERT_EQ(A(0, 1), C(0, 1));
}

TEST(MulMatrix, test1) {
  S21Matrix A(3, 1);
  A(0, 0) = 1;
  A(1, 0) = 2;
  A(2, 0) = 3;

  S21Matrix B(1, 3);
  B(0, 0) = 0.5;
  B(0, 1) = 0.75;
  B(0, 2) = 5;

  S21Matrix C = A * B;

  ASSERT_DOUBLE_EQ(C(0, 0), 0.5);
  ASSERT_DOUBLE_EQ(C(0, 1), 0.75);
  ASSERT_DOUBLE_EQ(C(0, 2), 5);
  ASSERT_DOUBLE_EQ(C(1, 0), 1);
  ASSERT_DOUBLE_EQ(C(1, 1), 1.5);
  ASSERT_DOUBLE_EQ(C(1, 2), 10);
  ASSERT_DOUBLE_EQ(C(2, 0), 1.5);
  ASSERT_DOUBLE_EQ(C(2, 1), 2.25);
  ASSERT_DOUBLE_EQ(C(2, 2), 15);

  ASSERT_EQ(C.GetRows(), 3);
  ASSERT_EQ(C.GetCols(), 3);
}

TEST(mul_matrix, test3) {
  S21Matrix A(3, 1);
  A(0, 0) = 1;
  A(1, 0) = 2;
  A(2, 0) = 3;

  S21Matrix B(1, 3);
  B(0, 0) = 0.5;
  B(0, 1) = 0.75;
  B(0, 2) = 5;

  S21Matrix C = B * A;

  ASSERT_DOUBLE_EQ(C(0, 0), 17);
  ASSERT_EQ(C.GetRows(), 1);
  ASSERT_EQ(C.GetCols(), 1);
}

TEST(mul_matrix, test4) {
  S21Matrix A(3, 1);
  A(0, 0) = 1;
  A(1, 0) = 2;
  A(2, 0) = 3;

  S21Matrix B(1, 3);
  B(0, 0) = 0.5;
  B(0, 1) = 0.75;
  B(0, 2) = 5;

  B *= A;

  ASSERT_DOUBLE_EQ(B(0, 0), 17);
  ASSERT_EQ(B.GetRows(), 1);
  ASSERT_EQ(B.GetCols(), 1);
}

TEST(transpose, test1) {
  S21Matrix A(1, 4);
  A(0, 0) = 1;
  A(0, 1) = 2;
  A(0, 2) = 3;
  A(0, 3) = 4;

  S21Matrix B = A.Transpose();

  S21Matrix C(4, 1);
  C(0, 0) = 1;
  C(1, 0) = 2;
  C(2, 0) = 3;
  C(3, 0) = 4;
  ASSERT_TRUE(B.EqMatrix(C));
}

TEST(transpose, test2) {
  S21Matrix A(2, 2);
  A(0, 0) = 1;
  A(0, 1) = 2;
  A(1, 0) = 3;
  A(1, 1) = 4;

  S21Matrix B = A.Transpose();

  S21Matrix C(2, 2);
  C(0, 0) = 1;
  C(0, 1) = 3;
  C(1, 0) = 2;
  C(1, 1) = 4;
  ASSERT_TRUE(B.EqMatrix(C));
}

TEST(calc_complements, test1) {
  S21Matrix A(2, 2);
  A(0, 0) = 1;
  A(0, 1) = 2;
  A(1, 0) = 3;
  A(1, 1) = 4;

  S21Matrix B = A.CalcComplements();
  ASSERT_DOUBLE_EQ(B(0, 0), 4);
  ASSERT_DOUBLE_EQ(B(0, 1), -3);
  ASSERT_DOUBLE_EQ(B(1, 0), -2);
  ASSERT_DOUBLE_EQ(B(1, 1), 1);
}

TEST(calc_complements, test2) {
  S21Matrix A(2, 3);
  ASSERT_ANY_THROW(A.CalcComplements());

  S21Matrix B(1, 1);
  ASSERT_ANY_THROW(B.CalcComplements());
}

TEST(s21_determinant_check, test1) {
  S21Matrix A(2, 3);
  ASSERT_ANY_THROW(A.Determinant());
}

TEST(s21_determinant_check, test2) {
  S21Matrix A(5, 5);

  A(0, 0) = 0;
  A(0, 1) = 12;
  A(0, 2) = -12;
  A(0, 3) = 12;
  A(0, 4) = 6;

  A(1, 0) = -3;
  A(1, 1) = -9;
  A(1, 2) = 9;
  A(1, 3) = 9;
  A(1, 4) = -6;

  A(2, 0) = 0;
  A(2, 1) = 0;
  A(2, 2) = -2;
  A(2, 3) = 4;
  A(2, 4) = -2;

  A(3, 0) = -3;
  A(3, 1) = -17;
  A(3, 2) = 13;
  A(3, 3) = 3;
  A(3, 4) = -8;

  A(4, 0) = 0;
  A(4, 1) = 0;
  A(4, 2) = 4;
  A(4, 3) = -8;
  A(4, 4) = 0;

  ASSERT_DOUBLE_EQ(A.Determinant(), -1728);
}

TEST(inverse_check, test1) {
  S21Matrix a(3, 3), b(3, 3);

  a(0, 0) = 2;
  a(0, 1) = 5;
  a(0, 2) = 7;
  a(1, 0) = 6;
  a(1, 1) = 3;
  a(1, 2) = 4;
  a(2, 0) = 5;
  a(2, 1) = -2;
  a(2, 2) = -3;

  b(0, 0) = 1;
  b(0, 1) = -1;
  b(0, 2) = 1;
  b(1, 0) = -38;
  b(1, 1) = 41;
  b(1, 2) = -34;
  b(2, 0) = 27;
  b(2, 1) = -29;
  b(2, 2) = 24;

  a = a.InverseMatrix();

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ASSERT_DOUBLE_EQ(b(i, j), a(i, j));
    }
  }
}

TEST(inverse_check, test2) {
  S21Matrix A(3, 3);

  A(0, 0) = 6;
  A(0, 1) = -1;
  A(0, 2) = 12;
  A(1, 0) = -3;
  A(1, 1) = -5;
  A(1, 2) = -6;
  A(2, 0) = 1;
  A(2, 1) = 4;
  A(2, 2) = 2;

  ASSERT_ANY_THROW(A.InverseMatrix());
}

TEST(inverse_check, test3) {
  S21Matrix A(2, 3);
  ASSERT_ANY_THROW(A.InverseMatrix());
}

TEST(inverse_check, test4) {
  S21Matrix B(1, 1), A(1, 1);
  A(0, 0) = 2;
  B(0, 0) = 0.5;

  A = A.InverseMatrix();
  ASSERT_DOUBLE_EQ(B(0, 0), A(0, 0));
}

TEST(inverse_check, test5) {
  S21Matrix A(1, 1), B(1, 1);

  ASSERT_ANY_THROW(A.InverseMatrix());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}

void FillMatrix(S21Matrix &A) {
  int count = 0;
  for (int i = 0; i < A.GetRows(); i++) {
    for (int j = 0; j < A.GetCols(); j++) {
      if (i == j) {
        A(i, j) = count;
        count += 2;
      } else {
        A(i, j) = count++;
      }
    }
  }
}