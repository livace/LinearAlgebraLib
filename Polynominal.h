//
// Created by livace on 25.10.2018.
//

#ifndef LINEARALG_POLYNOMINAL_H
#define LINEARALG_POLYNOMINAL_H

#include <utility>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

template<typename T>
class Polynomial {
 public:
  Polynomial(const T &value = T(0))
          : coefficients_(1, value) {
  }

  explicit Polynomial(std::vector<T> coefficients)
          : coefficients_(std::move(coefficients)) {
    resize();
  }

  Polynomial(std::initializer_list<T> coefficients)
          : coefficients_(std::move(coefficients)) {
    resize();
  }

  template<typename Iter>
  explicit Polynomial(Iter begin, Iter end)
          : coefficients_(begin, end) {
    resize();
  }

  friend std::ostream &operator<<(std::ostream &out, const Polynomial &polynomial) {
    if (polynomial.Degree() == -1) {
      out << 0;
    } else {
      for (int i = polynomial.Degree(); i >= 0; i--) {
        polynomial.print_element(out, i, i == polynomial.Degree());
      }
    }
    return out;
  }

  int Degree() const {
    if (coefficients_.size() == 1 && coefficients_[0] == T(0)) {
      return -1;
    }
    return static_cast<int>(coefficients_.size()) - 1;
  }

  friend bool operator==(const Polynomial &lhs, const Polynomial &rhs) {
    return lhs.coefficients_ == rhs.coefficients_;
  }

  friend bool operator!=(const Polynomial &lhs, const Polynomial &rhs) {
    return !(lhs == rhs);
  }

  Polynomial &operator+=(const Polynomial &other) {
    while (coefficients_.size() < other.coefficients_.size()) {
      coefficients_.push_back(T(0));
    }

    for (int i = 0; i < static_cast<int>(coefficients_.size()); i++) {
      coefficients_[i] += other[i];
    }
    resize();
    return *this;
  }

  friend Polynomial operator+(const Polynomial &lhs, const Polynomial &rhs) {
    Polynomial result = lhs;
    result += rhs;
    return result;
  }

  Polynomial &operator-=(const Polynomial &other) {
    while (coefficients_.size() < other.coefficients_.size()) {
      coefficients_.push_back(T(0));
    }
    for (int i = 0; i < static_cast<int>(coefficients_.size()); i++) {
      coefficients_[i] -= other[i];
    }
    resize();
    return *this;
  }

  friend Polynomial operator-(const Polynomial &lhs, const Polynomial &rhs) {
    Polynomial result = lhs;
    result -= rhs;
    return result;
  }

  Polynomial &operator*=(const Polynomial &other) {
    std::vector<T> result_coefficients =
            std::vector<T>(coefficients_.size() + other.coefficients_.size());

    for (int i = 0; i < static_cast<int>(coefficients_.size()); i++) {
      for (int j = 0; j < static_cast<int>(other.coefficients_.size()); j++) {
        result_coefficients[i + j] += (*this)[i] * other[j];
      }
    }

    swap(coefficients_, result_coefficients);
    resize();

    return *this;
  }

  friend Polynomial operator*(const Polynomial &lhs, const Polynomial &rhs) {
    Polynomial result = lhs;
    result *= rhs;
    return result;
  }

  const T operator[](size_t degree) const {
    if (degree >= coefficients_.size()) {
      return T(0);
    }
    return coefficients_[degree];
  }

  auto begin() const {
    return coefficients_.begin();
  }

  auto end() const {
    return coefficients_.begin() + Degree() + 1;
  }

  Polynomial operator&(const Polynomial &other) const {
    Polynomial current_other = T(1);
    Polynomial result = T(0);
    for (int i = 0; i <= Degree(); i++) {
      result += (*this)[i] * current_other;
      current_other *= other;
    }
    return result;
  }

  T operator()(T x) const {
    T result = T(0);
    T current_power = T(1);
    for (int i = 0; i <= Degree(); i++) {
      result += (*this)[i] * current_power;
      current_power *= x;
    }
    return result;
  }

  Polynomial &operator/=(const Polynomial &other) {
    Polynomial result = T(0);
    for (int i = Degree() - other.Degree(); i >= 0; i--) {
      T current_coefficient =
              (*this)[i + other.Degree()] / other[other.Degree()];
      std::vector<T> current_degree_coefficients(static_cast<size_t >(i) + 1);
      current_degree_coefficients[i] = T(1);
      result += current_coefficient * Polynomial(current_degree_coefficients);
      *this -= current_coefficient * Polynomial(current_degree_coefficients) * other;
    }
    swap(coefficients_, result.coefficients_);
    return *this;
  }

  friend Polynomial operator/(const Polynomial &lhs, const Polynomial &rhs) {
    Polynomial result = lhs;
    result /= rhs;
    return result;
  }

  Polynomial &operator%=(const Polynomial &other) {
    *this -= (*this / other) * other;
    return *this;
  }

  friend Polynomial operator%(const Polynomial &lhs, const Polynomial &rhs) {
    Polynomial result = lhs;
    result %= rhs;
    return result;
  }

  friend Polynomial operator,(Polynomial lhs, Polynomial rhs) {
    while (rhs != Polynomial()) {
      lhs %= rhs;
      swap(lhs, rhs);
    }
    if (lhs.Degree() != -1) {
      lhs /= lhs[lhs.Degree()];
    }
    return lhs;
  }

  friend void swap(Polynomial &a, Polynomial &b) {
    std::swap(a.coefficients_, b.coefficients_);
  }

  Polynomial operator-() const {
    Polynomial tmp = *this;
    for (auto &item : tmp.coefficients_) {
      item = -item;
    }
    return tmp;
  }

 private:
  std::vector<T> coefficients_;

  void resize() {
    size_t new_size = coefficients_.size();
    for (int i = static_cast<int>(coefficients_.size()) - 1; i >= 0; i--) {
      if ((*this)[i] != T(0)) {
        break;
      }
      new_size--;
    }
    if (new_size == 0) {
      coefficients_.resize(1);
    } else {
      coefficients_.resize(new_size);
    }
  }

  void print_element(std::ostream &out, int power, bool is_first) const {
    if ((*this)[power] == T(0)) {
      return;
    }
    print_plus_if_needed(out, power, is_first);
    print_coefficient(out, power);
    print_dot_if_needed(out, power);
    print_x(out, power);
  }

  void print_plus_if_needed(std::ostream &out, int power, bool is_first) const {
    if ((*this)[power] >= T(0) && !is_first) {
      out << "+";
    }
  }

  void print_coefficient(std::ostream &out, int power) const {
    const T &value = (*this)[power];
    if (power == 0) {
      if (value != T(0)) {
        out << value;
      }
      return;
    }
    if (value == T(1)) {
      return;
    } else if (value == T(-1)) {
      out << "-";
    } else {
      out << value;
    }
  }

  void print_dot_if_needed(std::ostream &out, int power) const {
    const T &value = (*this)[power];
    bool lhs_printed = false;
    if (value != T(1) && value != T(0) && value != T(-1)) {
      lhs_printed = true;
    }

    bool rhs_printed = false;
    if (power != 0) {
      rhs_printed = true;
    }

    if (lhs_printed && rhs_printed) {
      out << "*";
    }
  }

  void print_x(std::ostream &out, int power) const {
    if (power == 0) {
      return;
    } else if (power == 1) {
      out << "x";
    } else {
      out << "x^" << power;
    }
  }
};

#endif //LINEARALG_POLYNOMINAL_H
