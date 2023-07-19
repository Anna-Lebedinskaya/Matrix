#include "s21_matrix_oop.h"

// конструкторы, деструктор

S21Matrix::S21Matrix() : S21Matrix(2, 2) {} // делегирующий конструктор

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows_ < 1 || cols_ < 1)
    throw std::invalid_argument("Error: invalid matrix sizes");
  CreateMatrix();
}

S21Matrix::S21Matrix(const S21Matrix &other)
    : S21Matrix(other.rows_, other.cols_) {
  *this = other;
}

S21Matrix::S21Matrix(S21Matrix &&other) : rows_(0), cols_(0), matrix_(nullptr) {
  std::swap(rows_, other.rows_);
  std::swap(cols_, other.cols_);
  std::swap(matrix_, other.matrix_);
}

S21Matrix::~S21Matrix() { DelMatrix(); }

// accessor, mutator

int S21Matrix::GetRows() const { return rows_; }

int S21Matrix::GetCols() const { return cols_; }

void S21Matrix::SetRowsCols(int k, bool flag) { // 0 - rows, 1-cols
  if (k < 1)
    throw std::invalid_argument("Error: invalid parameter");
  int newSizeCols = cols_;
  int newSizeRows = rows_;
  int iMax = rows_;
  int jMax = cols_;
  if (flag) {
    newSizeCols = k;
    if (k < cols_) {
      jMax = k;
    }
  } else {
    newSizeRows = k;
    if (k < rows_) {
      iMax = k;
    }
  }
  S21Matrix tmpMatrix(newSizeRows, newSizeCols);
  for (int i = 0; i < iMax; i++) {
    for (int j = 0; j < jMax; j++) {
      tmpMatrix.matrix_[i][j] = matrix_[i][j];
    }
  }
  *this = tmpMatrix;
}

//математические операции над матрицами

bool S21Matrix::EqMatrix(const S21Matrix &other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    return false;
  if (this != &other) {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        if (fabs(matrix_[i][j] - other.matrix_[i][j]) > 0.0000001) {
          return false;
        }
      }
    }
  }
  return true;
}

void S21Matrix::SumMatrix(const S21Matrix &other) {
  CheckMatrixSizes(other);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix &other) {
  CheckMatrixSizes(other);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix &other) {
  if (cols_ != other.rows_)
    throw std::invalid_argument(
        "Error: the number of columns of the first matrix is not equal to the "
        "number of rows of the second matrix");
  S21Matrix newMatrix(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      for (int k = 0; k < cols_; k++) {
        newMatrix.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  *this = newMatrix;
}

S21Matrix S21Matrix::Transpose() const {
  S21Matrix newMatrix(cols_, rows_);
  for (int i = 0; i < newMatrix.rows_; i++) {
    for (int j = 0; j < newMatrix.cols_; j++) {
      newMatrix.matrix_[i][j] = matrix_[j][i];
    }
  }
  return newMatrix;
}

double S21Matrix::Determinant() const {
  CheckMatrixSquare();
  double determinant = 0.0;
  if (rows_ == 1) {
    determinant = matrix_[0][0];
  } else if (rows_ == 2) {
    determinant = matrix_[0][0] * matrix_[1][1] - matrix_[1][0] * matrix_[0][1];
  } else if (rows_ > 2) {
    for (int j = 0; j < cols_; j++) {
      S21Matrix tmpMatrix = LessMatrix(0, j);
      determinant +=
          matrix_[0][j] * tmpMatrix.Determinant() * ((j % 2 == 0) ? 1 : -1);
    }
  }
  return determinant;
}

S21Matrix S21Matrix::CalcComplements() {
  CheckMatrixSquare();
  S21Matrix newMatrix(rows_, cols_);
  for (int i = 0; i < newMatrix.rows_; i++) {
    for (int j = 0; j < newMatrix.cols_; j++) {
      S21Matrix tmpMatrix = LessMatrix(i, j);
      newMatrix.matrix_[i][j] =
          tmpMatrix.Determinant() * (((i + j) % 2 == 0) ? 1.0 : -1.0);
    }
  }
  return newMatrix;
}

S21Matrix S21Matrix::InverseMatrix() {
  double determinant = Determinant();
  if (determinant == 0)
    throw std::invalid_argument("Error: matrix determinant is 0");
  S21Matrix newMatrix(rows_, cols_);
  if (rows_ == 1) {
    newMatrix(0, 0) = 1 / matrix_[0][0];
  } else {
    newMatrix = CalcComplements().Transpose();
    newMatrix.MulNumber(1 / determinant);
  }
  return newMatrix;
}

//перегрузка операторов

S21Matrix S21Matrix::operator+(const S21Matrix &other) const {
  S21Matrix newMatrix = *this;
  newMatrix.SumMatrix(other);
  return newMatrix;
}

S21Matrix S21Matrix::operator-(const S21Matrix &other) const {
  S21Matrix newMatrix = *this;
  newMatrix.SubMatrix(other);
  return newMatrix;
}

S21Matrix S21Matrix::operator*(const double num) const {
  S21Matrix newMatrix = *this;
  newMatrix.MulNumber(num);
  return newMatrix;
}

S21Matrix S21Matrix::operator*(const S21Matrix &other) const {
  S21Matrix newMatrix = *this;
  newMatrix.MulMatrix(other);
  return newMatrix;
}

S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  if (this != &other) {
    DelMatrix();
    this->rows_ = other.rows_;
    this->cols_ = other.cols_;
    CreateMatrix();
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
  return *this;
}

bool S21Matrix::operator==(const S21Matrix &other) const {
  return EqMatrix(other);
}

S21Matrix &S21Matrix::operator+=(const S21Matrix &other) {
  SumMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator-=(const S21Matrix &other) {
  SubMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator*=(const double num) {
  MulNumber(num);
  return *this;
}

S21Matrix &S21Matrix::operator*=(const S21Matrix &other) {
  MulMatrix(other);
  return *this;
}

double &S21Matrix::operator()(int row, int col) {
  if (row >= rows_ || row < 0 || col >= cols_ || col < 0) {
    throw std::invalid_argument("Error: invalid parameters");
  }
  return matrix_[row][col];
}

//вспомогательные функции
void S21Matrix::CheckMatrixSizes(const S21Matrix &other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::invalid_argument("Error: invalid matrix sizes");
}

void S21Matrix::CheckMatrixSquare() const {
  if (rows_ != cols_)
    throw std::invalid_argument("Error: matrix is not square");
}

void S21Matrix::CreateMatrix() {
  matrix_ = new double *[rows_];
  for (int i = 0; i < rows_; i++)
    matrix_[i] =
        new double[cols_](); // new double[аргумент]() - скобки зануляют память
}

void S21Matrix::DelMatrix() {
  if (matrix_) {
    for (int i = 0; i < rows_; i++) {
      if (matrix_[i])
        delete[] matrix_[i];
    }
    delete[] matrix_;
    matrix_ = nullptr;
  }
}

S21Matrix S21Matrix::LessMatrix(int numRow, int numCol) const {
  S21Matrix newMatrix(rows_ - 1, cols_ - 1);
  for (int i = 0; i < newMatrix.rows_; i++) {
    for (int j = 0; j < newMatrix.cols_; j++) {
      if (i < numRow && j < numCol) {
        newMatrix.matrix_[i][j] = matrix_[i][j];
      } else if (i >= numRow && j < numCol) {
        newMatrix.matrix_[i][j] = matrix_[i + 1][j];
      } else if (i >= numRow && j >= numCol) {
        newMatrix.matrix_[i][j] = matrix_[i + 1][j + 1];
      } else if (i < numRow && j >= numCol) {
        newMatrix.matrix_[i][j] = matrix_[i][j + 1];
      }
    }
  }
  return newMatrix;
}
