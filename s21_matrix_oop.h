#ifndef S21_MATRIX_OOP_H_
#define S21_MATRIX_OOP_H_

#include <cmath>
#include <cstring>
#include <iostream>

class S21Matrix {
private:
  int rows_, cols_;
  double **matrix_;

  //вспомогательные функции
  void CheckMatrixSizes(const S21Matrix &other) const;
  void CheckMatrixSquare() const;
  void CreateMatrix();
  void DelMatrix();
  S21Matrix LessMatrix(int numRow, int numCol) const;

public:
  // конструкторы, деструктор
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix &other);
  S21Matrix(S21Matrix &&other);
  ~S21Matrix();

  // accessor, mutator
  int GetRows() const;
  int GetCols() const;
  void SetRowsCols(int k, bool flag);

  //математические операции над матрицами
  bool EqMatrix(const S21Matrix &other) const;
  void SumMatrix(const S21Matrix &other);
  void SubMatrix(const S21Matrix &other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix &other);
  S21Matrix Transpose() const;
  double Determinant() const;
  S21Matrix CalcComplements();
  S21Matrix InverseMatrix();

  //перегрузка операторов
  S21Matrix operator+(const S21Matrix &other) const;
  S21Matrix operator-(const S21Matrix &other) const;
  S21Matrix operator*(const double num) const;
  S21Matrix operator*(const S21Matrix &other) const;
  S21Matrix &operator=(const S21Matrix &other);
  bool operator==(const S21Matrix &other) const;
  S21Matrix &operator+=(const S21Matrix &other);
  S21Matrix &operator-=(const S21Matrix &other);
  S21Matrix &operator*=(const double num);
  S21Matrix &operator*=(const S21Matrix &other);
  double &operator()(int row, int col);
};

#endif
