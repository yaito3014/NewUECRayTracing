#pragma once

#ifndef YK_RAYTRACING_MATH_HPP
#define YK_RAYTRACING_MATH_HPP

#include <limits>

namespace yk::math {

// namespace detail {
//
// template <class T>
// constexpr T sqrtNewtonRaphson(T x, T curr, T prev) noexcept {
//  return curr == prev ? curr
//                      : sqrtNewtonRaphson(x, (curr + x / curr) / T(2), curr);
//}
//
//}  // namespace detail
//
// template <class T>
// constexpr T sqrt(T x) noexcept {
//  return x >= 0 && x < std::numeric_limits<T>::infinity()
//             ? detail::sqrtNewtonRaphson<T>(x, x, 0)
//             : std::numeric_limits<T>::quiet_NaN();
//}
//
// template <class T>
// constexpr T hypot(T x, T y, T z) noexcept {
//  return sqrt(x * x + y * y + z * z);
//}

template <class T>
constexpr T abs(T x) noexcept {
  return x > 0 ? x : -x;
}

template <class T>
constexpr T pow(T a, unsigned n) noexcept {
  T res = 1;
  while (n) {
    if (n & 1) res *= a;
    if (n >>= 1) a *= a;
  }
  return res;
}

using std::acos;
using std::atan2;
using std::cos;
using std::floor;
using std::hypot;
using std::sin;
using std::sqrt;
using std::tan;
using std::log;

namespace numbers {

inline constexpr double pi = 3.14159265358979;

}  // namespace numbers

}  // namespace yk::math

#endif  // !YK_RAYTRACING_MATH_HPP
