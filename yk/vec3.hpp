#pragma once

#ifndef YK_RAYTRACING_VEC3_HPP
#define YK_RAYTRACING_VEC3_HPP

#include <algorithm>
#include <functional>
#include <limits>
#include <random>

#include "math.hpp"
#include "random.hpp"

namespace yk {

template <class T>
struct alignas(T) vec3 {
  T x, y, z;

  constexpr vec3& operator+=(const vec3& rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  constexpr vec3& operator-=(const vec3& rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  constexpr vec3& operator*=(const vec3<T>& rhs) noexcept {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
  }

  template <class U>
  constexpr vec3& operator*=(const U& t) noexcept {
    x *= t;
    y *= t;
    z *= t;
    return *this;
  }

  template <class U>
  constexpr vec3& operator/=(const U& t) noexcept {
    x /= t;
    y /= t;
    z /= t;
    return *this;
  }

  constexpr T dot(const vec3& rhs) const noexcept {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }

  constexpr auto cross(const vec3& rhs) const noexcept {
    return vec3{
        y * rhs.z - z * rhs.y,
        z * rhs.x - x * rhs.z,
        x * rhs.y - y * rhs.x,
    };
  }

  friend constexpr vec3 operator-(const vec3& vec) noexcept {
    return {-vec.x, -vec.y, -vec.z};
  }

  friend constexpr vec3 operator+(const vec3& lhs, const vec3& rhs) noexcept {
    return vec3(lhs) += rhs;
  }

  friend constexpr vec3 operator-(const vec3& lhs, const vec3& rhs) noexcept {
    return vec3(lhs) -= rhs;
  }

  friend constexpr vec3 operator*(const vec3& lhs, const vec3& rhs) noexcept {
    return vec3(lhs) *= rhs;
  }

  template <class U>
  friend constexpr vec3 operator*(const vec3& vec, const U& scaler) noexcept {
    return vec3(vec) *= scaler;
  }

  template <class U>
  friend constexpr vec3 operator*(const U& scaler, const vec3& vec) noexcept {
    return vec3(vec) *= scaler;
  }

  template <class U>
  friend constexpr vec3 operator/(const vec3& vec, const U& scaler) noexcept {
    return vec3(vec) /= scaler;
  }
  template <class U>
  friend constexpr vec3 operator/(const U& scaler, const vec3& vec) noexcept {
    return vec3(vec) /= scaler;
  }

  constexpr T length_squared() const noexcept { return x * x + y * y + z * z; }

  constexpr T length() const noexcept { return math::hypot(x, y, z); }

  constexpr vec3& normalize() noexcept { return *this /= length(); }

  constexpr vec3 normalized() const noexcept { return *this / length(); }

  constexpr bool near_zero() const noexcept {
    constexpr auto s = std::numeric_limits<T>::epsilon();
    return math::abs(x) < s && math::abs(y) < s && math::abs(z) < s;
  }

  template <class Gen>
  static constexpr vec3<T> random(T min, T max, Gen& gen) noexcept {
    uniform_real_distribution<T> dist(min, max);
    return {
        dist(gen),
        dist(gen),
        dist(gen),
    };
  }

  template <class Gen>
  static constexpr vec3<T> random(Gen& gen) noexcept {
    return random(0, 1, gen);
  }
};

template <class T>
constexpr T dot(const vec3<T>& lhs, const vec3<T>& rhs) noexcept {
  return vec3<T>(lhs).dot(rhs);
}

template <class T>
constexpr vec3<T> cross(const vec3<T>& lhs, const vec3<T>& rhs) noexcept {
  return vec3<T>(lhs).cross(rhs);
}

template <class T>
constexpr vec3<T> reflect(const vec3<T>& v, const vec3<T>& n) noexcept {
  return v - 2 * dot(v, n) * n;
}

template <class T>
constexpr vec3<T> refract(const vec3<T>& uv, const vec3<T>& n,
                          T etai_over_etat) noexcept {
  auto cos_theta = std::min<T>(dot(-uv, n), 1);
  vec3<T> r_out_perp = etai_over_etat * (uv + cos_theta * n);
  vec3<T> r_out_parallel =
      -math::sqrt(math::abs(1 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}

template <class T, class Gen>
constexpr vec3<T> random_in_unit_sphere(Gen& gen) {
  uniform_real_distribution<T> dist(0, 0.999);
  return vec3<T>::random(-1, 1, gen).normalized() * dist(gen);
}

template <class T, class Gen>
constexpr vec3<T> random_unit_vector(Gen& gen) {
  return vec3<T>::random(-1, 1, gen).normalized();
}

template <class T, class Gen>
constexpr vec3<T> random_in_hemisphere(const vec3<T>& normal, Gen& gen) {
  vec3<T> in_unit_sphere = random_in_unit_sphere<T>(gen);
  return dot(in_unit_sphere, normal) > 0.0 ? in_unit_sphere : -in_unit_sphere;
}

template <class T, class Gen>
vec3<T> random_in_unit_disk(Gen& gen) {
  uniform_real_distribution<T> dist1(-1, 1);
  uniform_real_distribution<T> dist2(0, 0.99);
  return vec3<T>{dist1(gen), dist1(gen), 0}.normalized() * dist2(gen);
}

}  // namespace yk

#endif  // !YK_RAYTRACING_VEC3_HPP
