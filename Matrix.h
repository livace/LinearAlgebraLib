//
// Created by livace on 25.10.2018.
//

#ifndef LINEARALG_MATRIX_H
#define LINEARALG_MATRIX_H

#include <iostream>
#include <vector>
#include "Permutation.h"
#include "Polynominal.h"

template<typename T>
class Matrix;

template<typename T>
Matrix<T> Identity(int);

template<typename T>
T my_abs(T value) {
  if (value < 0) {
    return -value;
  }
  return value;
}

template<typename T>
class Matrix {
 private:
  std::vector<std::vector<T>> data_;

 public:
  size_t vertical_size() const {
    return data_.size();
  }

  size_t horizontal_size() const {
    if (!data_.empty()) {
      return data_[0].size();
    } else {
      return 0;
    }
  }

  Matrix() {

  }

  Matrix(int horizontal_size, int vertical_size) {
    data_.resize(vertical_size);
    for (auto &line : data_) {
      line.resize(horizontal_size);
    }
  }

  Matrix(const std::vector<std::vector<T>> &data) : data_(data) {
  }

  Matrix(std::initializer_list<std::initializer_list<T>> data) {
    for (auto line : data) {
      data_.push_back(std::vector<T>());
      for (auto item : line) {
        data_.back().push_back(item);
      }
    }
  }

  std::pair<size_t, size_t> size() const {
    return {vertical_size(), horizontal_size()};
  }

  friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix) {
    out << "(";
    for (size_t i = 0; i < matrix.size().first; i++) {
      if (i != 0) {
        out << ",\n";
      }
      out << "(";
      for (size_t j = 0; j < matrix.size().second; j++) {
        if (j != 0) {
          out << ", ";
        }
        out << matrix.data_[i][j];
      }
      out << ")";
    }
    out << ")";
    return out;
  }

  const std::vector<T> &operator[](int position) const {
    return data_[position];
  }

  std::vector<T> &operator[](int position) {
    return data_[position];
  }

  Matrix &operator+=(const Matrix &other) {
    for (size_t i = 0; i < vertical_size(); i++) {
      for (size_t j = 0; j < horizontal_size(); j++) {
        this->data_[i][j] += other.data_[i][j];
      }
    }
    return *this;
  }

  Matrix operator+(const Matrix &other) const {
    Matrix tmp = Matrix(*this);
    tmp += other;
    return tmp;
  }

  Matrix &operator-=(const Matrix &other) {
    (*this) += (-other);
    return *this;
  }

  Matrix operator-(const Matrix &other) const {
    return (*this) + (-other);
  }

  template<typename S>
  Matrix operator*=(const S &scalar) {
    for (size_t i = 0; i < vertical_size(); i++) {
      for (size_t j = 0; j < horizontal_size(); j++) {
        this->data_[i][j] *= scalar;
      }
    }
    return *this;
  }

  template<typename S>
  Matrix operator*(const S &scalar) const {
    Matrix tmp = Matrix(*this);
    tmp *= scalar;
    return tmp;
  }

  Matrix &operator*=(const Matrix &other) {
    std::vector<std::vector<T>> new_data(vertical_size());
    for (auto &line : new_data) {
      line.resize(other.horizontal_size());
    }

    Matrix other_transposed = other.transposed();
    for (size_t i = 0; i < vertical_size(); i++) {
      for (size_t j = 0; j < other.horizontal_size(); j++) {
        T sum = T(0);
        for (size_t k = 0; k < horizontal_size(); k++) {
          sum += data_[i][k] * other_transposed.data_[j][k];
        }
        new_data[i][j] = sum;
      }
    }
    data_ = new_data;
    return *this;
  }

  Matrix operator*(const Matrix &other) const {
    Matrix tmp = Matrix(*this);
    tmp *= other;
    return tmp;
  }

  Matrix &transpose() {
    std::vector<std::vector<T>> new_data(horizontal_size());
    for (auto &line : new_data) {
      line.resize(vertical_size());
    }
    for (size_t i = 0; i < vertical_size(); i++) {
      for (size_t j = 0; j < horizontal_size(); j++) {
        new_data[j][i] = data_[i][j];
      }
    }
    data_ = new_data;
    return *this;
  }

  Matrix transposed() const {
    Matrix tmp = *this;
    tmp.transpose();
    return tmp;
  }

  Matrix &make_gauss() {
    for (size_t column = 0, row = 0; row < vertical_size() && column < horizontal_size(); row++, column++) {
      size_t max_element_row = row;
      T coefficient = T(0);
      while (coefficient == T(0) && column < horizontal_size()) {
        for (size_t j = row + 1; j < vertical_size(); j++) {
          if (my_abs(data_[j][column]) > my_abs(data_[max_element_row][column])) {
            max_element_row = j;
          }
        }
        coefficient = data_[max_element_row][column];
        column++;
      }
      column--;
      std::swap(data_[row], data_[max_element_row]);

      for (size_t k = 0; k < horizontal_size(); k++) {
        data_[row][k] /= coefficient;
      }
      for (size_t j = 0; j < vertical_size(); j++) {
        if (j == row) continue;
        T current_coefficient = data_[j][column];
        for (size_t k = 0; k < horizontal_size(); k++) {
          data_[j][k] -= data_[row][k] * current_coefficient;
        }
      }
    }
    return *this;
  }

  T slow_determinant() const {
    Permutation permutation = Permutation(vertical_size());
    T result = T(0);
    do {
      T value = T(permutation.sign());
      for (int i = 0; i < vertical_size(); i++) {
        value *= (*this)[i][permutation[i]];
      }
      result += value;
    } while (permutation.next());
    return result;
  }

  Matrix gauss() const {
    Matrix copy = *this;
    copy.make_gauss();
    return copy;
  }

  template<typename U>
  std::vector<U> solve(const std::vector<U> &b) {
    std::vector<std::vector<U>> copied_data;
    for (const auto &line : data_) {
      copied_data.emplace_back(line.begin(), line.end());
    }
    for (size_t i = 0; i < vertical_size(); i++) {
      copied_data[i].push_back(b[i]);
    }

    std::vector<U> result = Matrix(copied_data).gauss().data_;
    for (size_t i = 0; i < vertical_size(); i++) {
      result.push_back(copied_data[i].back());
    }
    return result;
  }

  Matrix &resize_vertically(int new_size) {
    data_.resize(new_size, std::vector<T>(horizontal_size()));
  }

  Matrix &resize_horizontally(int new_size) {
    for (auto line : data_) {
      line.resize(new_size);
    }
  }

  Matrix &cut(int top, int left, int bottom, int right) {
    auto new_data = std::vector<std::vector<T>>(bottom - top);
    for (int i = top; i < bottom; i++) {
      new_data[i - top] = std::vector<T>(right - left);
      for (int j = left; j < right; j++) {
        new_data[i - top][j - left] = data_[i][j];
      }
    }
    data_ = new_data;
    return *this;
  }

  Matrix &cut(int vertical_size, int horizontal_size) {
    return cut(0, 0, vertical_size, horizontal_size);
  }

  Matrix cutted(int top, int left, int bottom, int right) const {
    Matrix result = *this;
    result.cut(top, left, bottom, right);
    return result;
  }

  Matrix cutted(int vertical_size, int horizontal_size) const {
    Matrix result = *this;
    result.cut(vertical_size, horizontal_size);
    return result;
  }

  Matrix &operator|=(const Matrix &other) {
    if (other.vertical_size() > vertical_size()) {
      resize_vertically(other.vertical_size());
    }
    for (int i = 0; i < vertical_size(); i++) {
      for (auto item : other[i]) {
        data_[i].push_back(item);
      }
    }
  }

  Matrix operator|(const Matrix &other) const {
    Matrix result = *this;
    result |= other;
    return result;
  }

  bool empty() {
    return data_.empty();
  }

  Matrix &inverse() {
    Matrix result = (*this) | Identity<T>(vertical_size());
    result.make_gauss();
    if (result[vertical_size() - 1][vertical_size() - 1] == T(0)) {
      data_.clear();
    } else {
      data_ = result.cut(0, vertical_size(), vertical_size(), vertical_size() * 2).data_;
    }
    return *this;
  }

  Matrix operator-() const {
    Matrix tmp = *this;
    for (auto &line : tmp.data_) {
      for (auto &item : line) {
        item = -item;
      }
    }
    return tmp;
  }

  Matrix inversed() const {
    Matrix result = *this;
    result.inverse();
    return result;
  }

  // Ожидается, что эту функцию не будут запускать от матрицы многчленов
  Polynomial<T> characteristic_polynomial() const {
    // result = lambda * E - A
    Matrix<Polynomial<T>> result(vertical_size(), horizontal_size());
    for (int i = 0; i < vertical_size(); i++) {
      result[i][i] = Polynomial<T>({0, 1});
    }
    // Сейчас result = lambda * E

    for (int i = 0; i < vertical_size(); i++) {
      for (int j = 0; j < horizontal_size(); j++) {
        // Делаем result = lambda * E - A
        // Нельзя просто сделать result -= *this, потому шаблонные типы разные
        result[i][j] -= (*this)[i][j];
      }
    }

    return result.slow_determinant();
  }
};

template<typename T>
Matrix<T> Identity(int size) {
  std::vector<std::vector<T>> data(static_cast<unsigned int>(size));
  for (int i = 0; i < size; i++) {
    data[i].resize(size, T(0));
    data[i][i] = T(1);
  }
  return Matrix<T>(data);
}

#endif //LINEARALG_MATRIX_H
