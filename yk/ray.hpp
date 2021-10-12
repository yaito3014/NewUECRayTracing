#pragma once

#ifndef YK_RAYTRACING_RAY_HPP
#define YK_RAYTRACING_RAY_HPP

#include "pos3.hpp"
#include "vec3.hpp"

namespace yk {

template <class T>
struct ray {
  pos3<T> origin;
  vec3<T> direction;

  template <class U>
  constexpr pos3<T> at(const U& scaler) const noexcept {
    return origin + scaler * direction;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_RAY_HPP
