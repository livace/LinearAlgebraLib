//
// Created by livace on 25.10.2018.
//

#ifndef LINEARALG_PERMUTATION_H
#define LINEARALG_PERMUTATION_H

#include <vector>
#include <ostream>
#include <algorithm>
#include <iostream>
#include "Utils.h"

class Permutation;

Permutation fast_pow(const Permutation &, int);

class Permutation {
 public:

  class LoopPrinter {
   private:
    const Permutation &permutation_;
    bool one_indexed_ = false;
    bool with_l_element_loops_ = false;
   public:
    LoopPrinter(const Permutation &permutation) : permutation_(permutation) {

    }

    LoopPrinter &one_indexed() {
      one_indexed_ = true;
      return *this;
    }

    LoopPrinter &zero_indexed() {
      one_indexed_ = false;
      return *this;
    }

    LoopPrinter &with_1_element_loops() {
      with_l_element_loops_ = true;
      return *this;
    }

    friend std::ostream &operator<<(std::ostream &out, const LoopPrinter &printer) {
      std::vector<bool> visited(printer.permutation_.size());
      for (int i = 0; i < printer.permutation_.size(); i++) {
        if (!visited[i]) {
          visited[i] = true;
          if (printer.permutation_[i] != i) {
            int j = printer.permutation_[i];
            int loop_size = 1;
            std::cout << "(";
            while (j != i) {
              if (j != printer.permutation_[i]) {
                std::cout << ", ";
              }
              int id = j;
              if (printer.one_indexed_) {
                id += 1;
              }
              std::cout << id;
              visited[j] = true;
              j = printer.permutation_[j];
              loop_size++;
            }
            int id = i;
            if (printer.one_indexed_) {
              id += 1;
            }
            out << ", " << id << ")";
          } else if (printer.with_l_element_loops_) {
            int id = i;
            if (printer.one_indexed_) {
              id += 1;
            }
            out << "(" << id << ")";
          }
        }
      }
      return out;
    }
  };

  Permutation(int size) {
    data_.resize(size);
    for (int i = 0; i < size; i++) {
      data_[i] = i;
    }
  }

  Permutation(const std::vector<int> &data) : data_(data) {

  }

  Permutation(std::initializer_list<int> data) : data_(data) {

  }

  Permutation operator*(const Permutation &other) const {
    Permutation lhs = *this;
    Permutation rhs = other;
    lhs.resize(rhs.size());
    rhs.resize(lhs.size());
    std::vector<int> new_data(lhs.size());
    for (int i = 0; i < lhs.size(); i++) {
      new_data[i] = lhs[rhs[i]];
    }
    return Permutation(new_data);
  }

  Permutation &operator*=(const Permutation &other) {
    Permutation result = (*this) * other;
    data_ = result.data_;
    return *this;
  }

  int operator[](int position) const {
    return data_[position];
  }

  void swap(int i, int j) {
    std::swap(data_[i], data_[j]);
  }

  size_t size() const {
    return data_.size();
  }

  void resize(int new_size) {
    int old_size = size();
    if (old_size >= new_size) {
      return;
    }
    data_.resize(static_cast<unsigned int>(new_size));
    for (int i = old_size; i < new_size; i++) {
      data_[i] = i;
    }
  }

  Permutation &pow(int power) {
    if (power == 0) {
      this->data_ = Permutation(size()).data_;
    } else {
      if (power < 0) {
        inverse();
        power = -power;
      }
      data_ = fast_pow(*this, power).data_;
    }
    return *this;
  }

  Permutation powered(int power) const {
    Permutation result(data_);
    return result.pow(power);
  }

  Permutation &inverse() {
    std::vector<int> new_data(size());
    for (int i = 0; i < size(); i++) {
      new_data[data_[i]] = i;
    }
    data_ = new_data;
    return *this;
  }

  Permutation inversed() const {
    Permutation result(data_);
    return result.inverse();
  }

  bool next() {
    int i = size() - 2;
    while (i >= 0 && data_[i] > data_[i + 1]) {
      i--;
    }
    if (i < 0) {
      return false;
    }
    int j = size() - 1;
    while (data_[j] < data_[i]) {
      j--;
    }
    std::swap(data_[j], data_[i]);
    std::reverse(data_.begin() + i + 1, data_.end());
    return true;
  }

  bool operator==(const Permutation &other) const {
    return data_ == other.data_;
  }

  bool operator!=(const Permutation &other) const {
    return data_ == other.data_;
  }

  bool operator<(const Permutation &other) const {
    return data_ < other.data_;
  }

  friend std::ostream &operator<<(std::ostream &out, Permutation permutation) {
    out << "(";
    for (int i = 0; i < permutation.size(); i++) {
      if (i != 0) {
        out << ", ";
      }
      out << permutation.data_[i];
    }
    out << ")";
    return out;
  }

  LoopPrinter loops() const {
    return LoopPrinter(*this);
  }

  int sign() const {
    std::vector<bool> visited(size());
    int result = 1;
    for (int i = 0; i < size(); i++) {
      if (!visited[i]) {
        visited[i] = true;
        int j = data_[i];
        int loop_size = 1;
        while (j != i) {
          visited[j] = true;
          j = data_[j];
          loop_size++;
        }
        if (loop_size % 2 == 0) {
          result = -result;
        }
      }
    }
    return result;
  }

 private:
  std::vector<int> data_;
};

Permutation fast_pow(const Permutation &value, int power) {
  Permutation current_power = value;
  Permutation result = Permutation(value.size());
  while (power != 0) {
    if (power % 2 == 1) {
      result *= current_power;
    }
    current_power *= current_power;
    power /= 2;
  }
  return result;
}

Permutation Loop(int size, std::vector<int> loop) {
  Permutation result(size);
  for (int i = 0; i + 1 < loop.size(); i++) {
    result.swap(loop[i], loop[i + 1]);
  }
  return result;
}

Permutation Loop(int size, std::initializer_list<int> loop) {
  return Loop(size, std::vector<int>(loop));
}

#endif //LINEARALG_PERMUTATION_H
