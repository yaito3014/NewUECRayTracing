#pragma once

#ifndef YK_RAYTRACING_POS3_HPP
#define YK_RAYTRACING_POS3_HPP

#include "vec3.hpp"

namespace yk {

template <class T>
struct alignas(T) pos3 {
  T x, y, z;

  constexpr pos3& operator+=(const vec3<T>& rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  constexpr pos3& operator-=(const vec3<T>& rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  friend constexpr pos3 operator+(const pos3& lhs,
                                  const vec3<T>& rhs) noexcept {
    return pos3(lhs) += rhs;
  }

  friend constexpr pos3 operator-(const pos3& lhs,
                                  const vec3<T>& rhs) noexcept {
    return pos3(lhs) -= rhs;
  }

  friend constexpr vec3<T> operator-(const pos3& lhs,
                                     const pos3& rhs) noexcept {
    return {
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z,
    };
  }

  constexpr vec3<T> to_vec(const pos3& origin = {0, 0, 0}) const noexcept {
    return {x - origin.x, y - origin.y, z - origin.z};
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_POS3_HPP
