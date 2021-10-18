#pragma once

#ifndef YK_RAYTRACING_COLOR_HPP
#define YK_RAYTRACING_COLOR_HPP

#include "random.hpp"

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

  template <class Gen>
  static constexpr color random(T min, T max, Gen& gen) noexcept {
    uniform_real_distribution<T> dist(min, max);
    return {
        dist(gen),
        dist(gen),
        dist(gen),
    };
  }

  template <class Gen>
  static constexpr color random(Gen& gen) noexcept {
    return random(0, 1, gen);
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_COLOR_HPP
