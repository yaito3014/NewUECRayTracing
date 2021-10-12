#pragma once

#ifndef YK_RAYTRACING_COLOR_HPP
#define YK_RAYTRACING_COLOR_HPP

namespace yk {

template <class T>
struct alignas(T) color {
  T r, g, b;

  constexpr color& operator+=(const color& rhs) noexcept {
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    return *this;
  }

  template <class U>
  constexpr color& operator*=(const U& scaler) noexcept {
    r *= scaler;
    g *= scaler;
    b *= scaler;
    return *this;
  }

  constexpr color& operator*=(const color& rhs) noexcept {
    r *= rhs.r;
    g *= rhs.g;
    b *= rhs.b;
    return *this;
  }

  template <class U>
  constexpr color& operator/=(const U& scaler) noexcept {
    r /= scaler;
    g /= scaler;
    b /= scaler;
    return *this;
  }

  friend constexpr color operator+(const color& lhs,
                                   const color& rhs) noexcept {
    return color(lhs) += rhs;
  }

  template <class U>
  friend constexpr color operator*(const color& c, const U& scaler) noexcept {
    return color(c) *= scaler;
  }

  template <class U>
  friend constexpr color operator*(const U& scaler, const color& c) noexcept {
    return color(c) *= scaler;
  }

  friend constexpr color operator*(const color& lhs,
                                   const color& rhs) noexcept {
    return color(lhs) *= rhs;
  }

  template <class U>
  friend constexpr color operator/(const color& c, const U& scaler) noexcept {
    return color(c) /= scaler;
  }

  template <class U>
  friend constexpr color operator/(const U& scaler, const color& c) noexcept {
    return color(c) /= scaler;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_COLOR_HPP
