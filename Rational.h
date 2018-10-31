//
// Created by livace on 25.10.2018.
//

#ifndef LINEARALG_RATIONAL_H
#define LINEARALG_RATIONAL_H

#include <iostream>
#include <vector>
#include <algorithm>

class Rational {
 public:
  using value_type = long long;

  Rational(value_type numerator = 0, value_type denominator = 1)
          : numerator_(numerator), denominator_(denominator) {
    normalize();
  }

  Rational &operator+=(Rational other) {
    value_type new_numerator = other.numerator_ * denominator_ + numerator_ * other.denominator_;
    denominator_ *= other.denominator_;
    numerator_ = new_numerator;
    normalize();
    return *this;
  }

  Rational &operator+=(value_type other) {
    return (*this += Rational(other));
  }

  Rational operator+(Rational other) const {
    Rational tmp = *this;
    tmp += other;
    return tmp;
  }

  Rational operator+(value_type other) {
    Rational tmp = *this;
    tmp += other;
    return tmp;
  }

  friend Rational operator+(value_type lhs, Rational rhs) {
    return Rational(lhs) + rhs;
  }

  Rational &operator*=(Rational other) {
    denominator_ *= other.denominator_;
    numerator_ *= other.numerator_;
    normalize();
    return *this;
  }

  Rational &operator*=(value_type other) {
    return (*this *= Rational(other));
  }

  Rational operator*(Rational other) const {
    Rational tmp = *this;
    tmp *= other;
    return tmp;
  }

  Rational operator*(value_type other) const {
    Rational tmp = *this;
    tmp *= other;
    return tmp;
  }

  friend Rational operator*(value_type lhs, Rational rhs) {
    return Rational(lhs) * rhs;
  }

  Rational &operator-=(Rational other) {
    value_type new_numerator = numerator_ * other.denominator_ - other.numerator_ * denominator_;
    denominator_ *= other.denominator_;
    numerator_ = new_numerator;
    normalize();
    return *this;
  }

  Rational &operator-=(value_type other) {
    return (*this -= Rational(other));
  }

  Rational operator-(Rational other) const {
    Rational tmp = *this;
    tmp -= other;
    return tmp;
  }

  Rational operator-(value_type other) const {
    Rational tmp = *this;
    tmp -= other;
    return tmp;
  }

  friend Rational operator-(value_type lhs, Rational rhs) {
    return Rational(lhs) - rhs;
  }

  Rational &operator/=(Rational other) {
    numerator_ *= other.denominator_;
    denominator_ *= other.numerator_;
    normalize();
    return *this;
  }

  Rational &operator/=(value_type other) {
    return (*this /= Rational(other));
  }

  Rational operator/(Rational other) const {
    Rational tmp = *this;
    tmp /= other;
    return tmp;
  }

  Rational operator/(value_type other) const {
    Rational tmp = *this;
    tmp /= other;
    return tmp;
  }

  friend Rational operator/(value_type lhs, Rational rhs) {
    return Rational(lhs) / rhs;
  }

  Rational operator-() const {
    return *this * -1;
  }

  Rational operator+() const {
    return *this;
  }

  const Rational operator++(int) {
    auto tmp = *this;
    *this += 1;
    return tmp;
  }

  const Rational operator--(int) {
    auto tmp = *this;
    *this -= 1;
    return tmp;
  }

  Rational &operator++() {
    *this += 1;
    return *this;
  }

  Rational &operator--() {
    *this -= 1;
    return *this;
  }

  bool operator<(const Rational &other) const {
    return numerator_ * other.denominator_ < other.numerator_ * denominator_;
  }

  bool operator==(const Rational &other) const {
    return numerator_ == other.numerator_ && denominator_ == other.denominator_;
  }

  bool operator<=(const Rational &other) const {
    return ((*this) == other) || (*this < other);
  }

  bool operator>(const Rational &other) const {
    return !((*this) <= other);
  }

  bool operator>=(const Rational &other) const {
    return !((*this) < other);
  }

  bool operator!=(const Rational &other) const {
    return !(*this == other);
  }

  value_type numerator() const {
    return numerator_;
  }

  value_type denominator() const {
    return denominator_;
  }

  friend std::ostream &operator<<(std::ostream &out, const Rational &rational) {
    out << rational.numerator_;
    if (rational.denominator_ != 1) {
      out << "/" << rational.denominator_;
    }
    return out;
  }

 private:
  long long numerator_;
  long long denominator_;

  void normalize() {
    value_type gcd = std::__gcd(numerator_, denominator_);
    numerator_ /= gcd;
    denominator_ /= gcd;
    if (denominator_ < 0) {
      numerator_ *= -1;
      denominator_ *= -1;
    }
  }
};

#endif //LINEARALG_RATIONAL_H
